#!/usr/bin/env python3
import argparse
import csv
import time
from collections import defaultdict
from pathlib import Path


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Set-semantics oracle for Line(k,a,b) over a Crown stream."
    )
    p.add_argument("--stream-crown", required=True, help="Path to +|-|* stream file")
    p.add_argument("--dataset-label", required=True, help="Dataset label for CSV output")
    p.add_argument("--k", type=int, required=True)
    p.add_argument("--out-csv", required=True)
    p.add_argument("--max-span", type=int, default=4, help="Max value of (b-a)")
    p.add_argument("--system", default="oracle")
    return p.parse_args()


def combos(k: int, max_span: int) -> list[tuple[int, int]]:
    out: list[tuple[int, int]] = []
    for a in range(1, k + 2):
        b_max = min(k + 1, a + max_span)
        for b in range(a, b_max + 1):
            out.append((a, b))
    return out


def fnv_hash(out_counts: dict[str, int]) -> int:
    h = 1469598103934665603
    for key in sorted(out_counts.keys()):
        for c in key.encode("utf-8"):
            h ^= c
            h = (h * 1099511628211) & 0xFFFFFFFFFFFFFFFF
        v = out_counts[key] & 0xFFFFFFFFFFFFFFFF
        h ^= v
        h = (h * 1099511628211) & 0xFFFFFFFFFFFFFFFF
    return h


def build_adj(rels: dict[int, set[tuple[int, int]]], k: int) -> dict[int, dict[int, list[int]]]:
    out: dict[int, dict[int, list[int]]] = {}
    for rid in range(1, k + 1):
        fwd: dict[int, list[int]] = defaultdict(list)
        for u, v in rels[rid]:
            fwd[u].append(v)
        out[rid] = fwd
    return out


def compute_left_counts(
    rels: dict[int, set[tuple[int, int]]], adj: dict[int, dict[int, list[int]]], a: int
) -> dict[int, int] | None:
    if a == 1:
        return None

    dp: dict[int, int] = defaultdict(int)
    for u, v in rels[1]:
        dp[v] += 1
    for rid in range(2, a):
        ndp: dict[int, int] = defaultdict(int)
        fwd = adj[rid]
        for src, cnt in dp.items():
            for dst in fwd.get(src, []):
                ndp[dst] += cnt
        dp = ndp
        if not dp:
            break
    return dp


def compute_right_counts(
    rels: dict[int, set[tuple[int, int]]], adj: dict[int, dict[int, list[int]]], k: int, b: int
) -> dict[int, int] | None:
    if b == k + 1:
        return None

    dp: dict[int, int] = defaultdict(int)
    for u, _ in rels[k]:
        dp[u] += 1
    for rid in range(k - 1, b - 1, -1):
        ndp: dict[int, int] = defaultdict(int)
        fwd = adj[rid]
        for src, dsts in fwd.items():
            total = 0
            for dst in dsts:
                total += dp.get(dst, 0)
            if total:
                ndp[src] += total
        dp = ndp
        if not dp:
            break
    return dp


def compute_group_counts(
    rels: dict[int, set[tuple[int, int]]], k: int, a: int, b: int
) -> dict[str, int]:
    adj = build_adj(rels, k)
    out_counts: dict[str, int] = defaultdict(int)

    left_counts = compute_left_counts(rels, adj, a)
    right_counts = compute_right_counts(rels, adj, k, b)

    if a == b:
        if a == 1:
            if right_counts is None:
                return {}
            for x1, cnt in right_counts.items():
                if cnt:
                    out_counts[str(x1)] += cnt
            return out_counts
        if a == k + 1:
            if left_counts is None:
                return {}
            for x, cnt in left_counts.items():
                if cnt:
                    out_counts[str(x)] += cnt
            return out_counts

        if left_counts is None or right_counts is None:
            return {}
        for x, lc in left_counts.items():
            rc = right_counts.get(x, 0)
            if rc:
                out_counts[str(x)] += lc * rc
        return out_counts

    # Enumerate middle segment x_a..x_b (edges a..b-1).
    def emit_path(values: list[int]) -> None:
        left_w = 1 if a == 1 else (left_counts.get(values[0], 0) if left_counts else 0)
        if left_w == 0:
            return
        right_w = (
            1 if b == k + 1 else (right_counts.get(values[-1], 0) if right_counts else 0)
        )
        if right_w == 0:
            return
        key = "|".join(str(x) for x in values)
        out_counts[key] += left_w * right_w

    def dfs(rid: int, prev: int, values: list[int]) -> None:
        if rid > b - 1:
            emit_path(values)
            return
        for nxt in adj[rid].get(prev, []):
            values.append(nxt)
            dfs(rid + 1, nxt, values)
            values.pop()

    for u, v in rels[a]:
        values = [u, v]
        dfs(a + 1, v, values)

    # Drop zeros (defensive for future non-count extensions).
    return {k_: v_ for k_, v_ in out_counts.items() if v_ != 0}


def main() -> None:
    args = parse_args()
    stream = Path(args.stream_crown).resolve()
    out_csv = Path(args.out_csv).resolve()

    if not stream.exists():
        raise FileNotFoundError(f"stream not found: {stream}")
    if args.k < 1:
        raise ValueError("k must be >= 1")

    rels: dict[int, set[tuple[int, int]]] = {rid: set() for rid in range(1, args.k + 1)}
    ab_combos = combos(args.k, args.max_span)

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

        phase_idx = 0
        first_phase = True
        window_ops = 0
        with stream.open("r", encoding="utf-8") as sf:
            for raw in sf:
                line = raw.strip()
                if not line:
                    continue

                if line == "*":
                    phase_name = "initial_load" if first_phase else "updates"
                    out_phase_idx = 0 if first_phase else phase_idx
                    for a, b in ab_combos:
                        t0 = time.perf_counter()
                        out_counts = compute_group_counts(rels, args.k, a, b)
                        elapsed_ms = (time.perf_counter() - t0) * 1000.0
                        sig = fnv_hash(out_counts)
                        rows = len(out_counts)
                        throughput = (
                            (window_ops * 1000.0 / elapsed_ms) if elapsed_ms > 0 else 0.0
                        )
                        w.writerow(
                            [
                                args.system,
                                args.dataset_label,
                                args.k,
                                a,
                                b,
                                phase_name,
                                out_phase_idx,
                                window_ops,
                                f"{elapsed_ms:.3f}",
                                f"{throughput:.3f}",
                                sig,
                                rows,
                            ]
                        )
                    f.flush()

                    first_phase = False
                    phase_idx += 1
                    window_ops = 0
                    continue

                op, rid_s, src_s, dst_s = line.split("|")
                rid = int(rid_s)
                src = int(src_s)
                dst = int(dst_s)
                tup = (src, dst)

                if op == "+":
                    rels[rid].add(tup)
                elif op == "-":
                    rels[rid].discard(tup)
                else:
                    raise ValueError(f"Invalid op in stream line: {line}")
                window_ops += 1

    print(f"Wrote {out_csv}")


if __name__ == "__main__":
    main()
