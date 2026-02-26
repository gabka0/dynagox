#!/usr/bin/env python3
import argparse
import csv
import shutil
import subprocess
import tempfile
import time
from pathlib import Path


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Run Flink SQL Line(k,a,b) over +/- stream with set semantics."
    )
    p.add_argument("--flink-sql-client", required=True, help="Path to sql-client.sh")
    p.add_argument("--stream-csv", required=True, help="Input stream CSV path")
    p.add_argument("--dataset-label", required=True, help="Dataset label for output CSV")
    p.add_argument("--k-min", type=int, default=2)
    p.add_argument("--k-max", type=int, default=6)
    p.add_argument("--max-span", type=int, default=4)
    p.add_argument("--out-csv", required=True)
    p.add_argument("--system", default="flink")
    p.add_argument("--work-dir", default="/tmp")
    return p.parse_args()


def combos(k_min: int, k_max: int, max_span: int) -> list[tuple[int, int, int]]:
    out: list[tuple[int, int, int]] = []
    for k in range(k_min, k_max + 1):
        for a in range(1, k + 2):
            b_max = min(k + 1, a + max_span)
            for b in range(a, b_max + 1):
                out.append((k, a, b))
    return out


def load_windows(stream_csv: Path) -> list[tuple[int, int]]:
    marks: list[int] = []
    with stream_csv.open("r", encoding="utf-8", newline="") as f:
        r = csv.DictReader(f)
        for row in r:
            if row.get("is_milestone") == "1":
                marks.append(int(row["seq"]))
    if not marks:
        raise ValueError("No milestones in stream csv")

    windows: list[tuple[int, int]] = []
    prev = -1
    for m in marks:
        ops = m - prev - 1
        windows.append((m, ops))
        prev = m
    return windows


def var_expr(k: int, idx: int) -> str:
    if idx == 1:
        return "r1.src"
    if idx == k + 1:
        return f"r{k}.dst"
    return f"r{idx}.src"


def build_sql(stream_csv: Path, sink_dir: Path, k: int, a: int, b: int) -> str:
    sink_cols = [f"x{i} BIGINT" for i in range(a, b + 1)]
    select_cols = [f"{var_expr(k, i)} AS x{i}" for i in range(a, b + 1)]
    group_cols = [var_expr(k, i) for i in range(a, b + 1)]

    sql = f"""SET 'execution.runtime-mode' = 'batch';
SET 'sql-client.execution.result-mode' = 'TABLEAU';
CREATE TEMPORARY TABLE updates (
  seq BIGINT,
  op STRING,
  rid INT,
  src BIGINT,
  dst BIGINT,
  is_milestone INT
) WITH (
  'connector' = 'filesystem',
  'path' = '{stream_csv.as_posix()}',
  'format' = 'csv',
  'csv.ignore-parse-errors' = 'true'
);

CREATE TEMPORARY VIEW milestones AS
SELECT
  seq AS mseq,
  ROW_NUMBER() OVER (ORDER BY seq) - 1 AS phase_idx
FROM updates
WHERE is_milestone = 1;

CREATE TEMPORARY VIEW states AS
SELECT
  m.phase_idx AS phase_idx,
  u.rid AS rid,
  u.src AS src,
  u.dst AS dst
FROM milestones m
JOIN updates u ON u.seq < m.mseq
WHERE u.op IN ('+','-')
GROUP BY m.phase_idx, u.rid, u.src, u.dst
HAVING SUM(CASE WHEN u.op = '+' THEN 1 ELSE -1 END) > 0;
"""

    for rid in range(1, k + 1):
        sql += (
            f"CREATE TEMPORARY VIEW r{rid} AS "
            f"SELECT phase_idx, src, dst FROM states WHERE rid = {rid};\n"
        )

    sink_schema = ",\n  ".join(["phase_idx BIGINT"] + sink_cols + ["cnt BIGINT"])
    sql += f"""
CREATE TEMPORARY TABLE sink (
  {sink_schema}
) WITH (
  'connector' = 'filesystem',
  'path' = '{sink_dir.as_posix()}',
  'format' = 'csv'
);
"""

    join_clauses = []
    for rid in range(2, k + 1):
        join_clauses.append(
            f"JOIN r{rid} ON r{rid-1}.phase_idx = r{rid}.phase_idx "
            f"AND r{rid-1}.dst = r{rid}.src"
        )

    select_prefix = ",\n  ".join(["r1.phase_idx"] + select_cols + ["COUNT(*) AS cnt"])
    group_by = ", ".join(["r1.phase_idx"] + group_cols)
    joins = "\n".join(join_clauses)

    sql += f"""
INSERT INTO sink
SELECT
  {select_prefix}
FROM r1
{joins}
GROUP BY {group_by};
"""
    return sql


def collect_sink_by_phase(sink_dir: Path, key_cols: int) -> dict[int, list[tuple[str, int]]]:
    by_phase: dict[int, dict[str, int]] = {}
    for p in sorted(sink_dir.rglob("*")):
        if not p.is_file():
            continue
        name = p.name
        if name.startswith(".") or name.endswith(".crc"):
            continue
        with p.open("r", encoding="utf-8", newline="") as f:
            r = csv.reader(f)
            for row in r:
                if len(row) != key_cols + 2:
                    continue
                phase_idx = int(row[0])
                key = "|".join(row[1 : 1 + key_cols])
                cnt = int(row[1 + key_cols])
                if phase_idx not in by_phase:
                    by_phase[phase_idx] = {}
                by_phase[phase_idx][key] = by_phase[phase_idx].get(key, 0) + cnt

    out: dict[int, list[tuple[str, int]]] = {}
    for phase_idx, mp in by_phase.items():
        items = list(mp.items())
        items.sort(key=lambda x: x[0])
        out[phase_idx] = items
    return out


def fnv_hash(items: list[tuple[str, int]]) -> int:
    h = 1469598103934665603
    for key, cnt in items:
        for c in key.encode("utf-8"):
            h ^= c
            h = (h * 1099511628211) & 0xFFFFFFFFFFFFFFFF
        h ^= cnt & 0xFFFFFFFFFFFFFFFF
        h = (h * 1099511628211) & 0xFFFFFFFFFFFFFFFF
    return h


def main() -> None:
    args = parse_args()
    stream_csv = Path(args.stream_csv).resolve()
    sql_client = Path(args.flink_sql_client).resolve()
    out_csv = Path(args.out_csv).resolve()
    work_dir = Path(args.work_dir).resolve()

    if not stream_csv.exists():
        raise FileNotFoundError(f"stream csv not found: {stream_csv}")
    if not sql_client.exists():
        raise FileNotFoundError(f"sql-client not found: {sql_client}")

    windows = load_windows(stream_csv)
    all_combos = combos(args.k_min, args.k_max, args.max_span)

    out_csv.parent.mkdir(parents=True, exist_ok=True)
    with out_csv.open("w", encoding="utf-8", newline="") as f:
        w = csv.writer(f)
        w.writerow(
            [
                "system",
                "dataset",
                "k",
                "a",
                "b",
                "phase",
                "phase_idx",
                "ops",
                "time_ms",
                "throughput",
                "hash",
                "rows",
            ]
        )

        total = len(all_combos)
        for i, (k, a, b) in enumerate(all_combos, start=1):
            print(f"[{i}/{total}] Running k={k} a={a} b={b}", flush=True)
            job_dir = Path(
                tempfile.mkdtemp(prefix=f"flink_set_k{k}_a{a}_b{b}_", dir=work_dir.as_posix())
            )
            sink_dir = job_dir / "sink"
            sql_path = job_dir / "job.sql"
            sql_path.write_text(build_sql(stream_csv, sink_dir, k, a, b), encoding="utf-8")

            t0 = time.perf_counter()
            proc = subprocess.run(
                [sql_client.as_posix(), "-f", sql_path.as_posix()],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.PIPE,
                text=True,
                check=False,
            )
            elapsed_ms = (time.perf_counter() - t0) * 1000.0
            if proc.returncode != 0:
                raise RuntimeError(f"Flink failed for (k={k},a={a},b={b}).\n{proc.stderr}")

            by_phase = collect_sink_by_phase(sink_dir, key_cols=(b - a + 1))
            split_ms = elapsed_ms / max(len(windows), 1)
            for m_idx, (_, ops) in enumerate(windows):
                phase = "initial_load" if m_idx == 0 else "updates"
                phase_idx = 0 if m_idx == 0 else m_idx
                items = by_phase.get(phase_idx, [])
                rows = len(items)
                sig = fnv_hash(items)
                throughput = (ops * 1000.0 / split_ms) if split_ms > 0 else 0.0
                w.writerow(
                    [
                        args.system,
                        args.dataset_label,
                        k,
                        a,
                        b,
                        phase,
                        phase_idx,
                        ops,
                        f"{split_ms:.3f}",
                        f"{throughput:.3f}",
                        sig,
                        rows,
                    ]
                )
            f.flush()
            shutil.rmtree(job_dir, ignore_errors=True)
            print(f"[{i}/{total}] Done k={k} a={a} b={b}", flush=True)

    print(f"Wrote {out_csv}")


if __name__ == "__main__":
    main()
