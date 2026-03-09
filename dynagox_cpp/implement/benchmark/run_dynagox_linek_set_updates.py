#!/usr/bin/env python3
import argparse
import subprocess
import sys
from pathlib import Path


CSV_HEADER = "system,dataset,k,a,b,phase,phase_idx,ops,time_ms,throughput,hash,rows"


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description=(
            "Run DynaGox bench_linek_count_stream over set-semantics stream and "
            "export benchmark CSV aligned with Flink/oracle schema."
        )
    )
    p.add_argument("--bench-exe", required=True, help="Path to bench_linek_count_stream executable")
    p.add_argument(
        "--stream-pattern",
        required=True,
        help=(
            "Path to Crown stream (.crown). Use '{k}' placeholder for per-k files, "
            "or pass a single concrete path."
        ),
    )
    p.add_argument("--dataset-label", required=True)
    p.add_argument("--k-min", type=int, default=2)
    p.add_argument("--k-max", type=int, default=6)
    p.add_argument("--max-span", type=int, default=4)
    p.add_argument("--mode", choices=["full", "delta"], default="full")
    p.add_argument("--linek-mode", choices=["raw", "count"], default="count")
    p.add_argument("--root-kind", default="auto")
    p.add_argument("--root-pos", type=int, default=0)
    p.add_argument("--system", default="dynagox")
    p.add_argument("--out-csv", required=True)
    return p.parse_args()


def combos(k_min: int, k_max: int, max_span: int) -> list[tuple[int, int, int]]:
    out: list[tuple[int, int, int]] = []
    for k in range(k_min, k_max + 1):
        for a in range(1, k + 2):
            b_max = min(k + 1, a + max_span)
            for b in range(a, b_max + 1):
                out.append((k, a, b))
    return out


def resolve_stream_path(pattern: str, k: int) -> Path:
    if "{k}" in pattern:
        return Path(pattern.format(k=k)).resolve()
    return Path(pattern).resolve()


def run_one(args: argparse.Namespace, k: int, a: int, b: int, with_header: bool) -> list[str]:
    stream = resolve_stream_path(args.stream_pattern, k)
    if not stream.exists():
        raise FileNotFoundError(f"stream not found for k={k}: {stream}")

    cmd = [
        str(Path(args.bench_exe).resolve()),
        "--stream",
        str(stream),
        "--dataset-label",
        args.dataset_label,
        "--system",
        args.system,
        "--mode",
        args.mode,
        "--linek-mode",
        args.linek_mode,
        "--k",
        str(k),
        "--a",
        str(a),
        "--b",
        str(b),
        "--root-kind",
        args.root_kind,
        "--root-pos",
        str(args.root_pos),
    ]
    if with_header:
        cmd.append("--csv-header")

    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        sys.stderr.write(proc.stdout)
        sys.stderr.write(proc.stderr)
        raise RuntimeError(
            f"bench failed for k={k},a={a},b={b} with exit code {proc.returncode}"
        )

    lines = [ln.strip() for ln in proc.stdout.splitlines() if ln.strip()]
    if not lines:
        raise RuntimeError(f"empty output for k={k},a={a},b={b}")
    return lines


def main() -> None:
    args = parse_args()

    bench = Path(args.bench_exe).resolve()
    if not bench.exists():
        raise FileNotFoundError(f"bench executable not found: {bench}")

    if args.k_min < 1 or args.k_max < args.k_min:
        raise ValueError("Invalid k range")

    out_csv = Path(args.out_csv).resolve()
    out_csv.parent.mkdir(parents=True, exist_ok=True)

    all_combos = combos(args.k_min, args.k_max, args.max_span)
    total = len(all_combos)

    header_written = False
    with out_csv.open("w", encoding="utf-8", newline="") as out:
        for i, (k, a, b) in enumerate(all_combos, start=1):
            print(f"[{i}/{total}] Running DynaGox k={k} a={a} b={b} mode={args.mode} linek_mode={args.linek_mode}", flush=True)
            lines = run_one(args, k, a, b, with_header=(not header_written))

            start_idx = 0
            if not header_written:
                if lines[0] != CSV_HEADER:
                    raise RuntimeError(f"Unexpected CSV header: '{lines[0]}'")
                out.write(lines[0] + "\n")
                header_written = True
                start_idx = 1
            elif lines[0] == CSV_HEADER:
                start_idx = 1

            for line in lines[start_idx:]:
                out.write(line + "\n")

    print(f"Wrote {out_csv}")


if __name__ == "__main__":
    main()
