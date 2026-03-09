#!/usr/bin/env python3
import argparse
import re
import subprocess
import tempfile
from collections import defaultdict
from pathlib import Path


DELTA_RE = re.compile(r"^([+-])\|([0-9]+(?:\|[0-9]+)*)$")


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Validate DynaGox delta-raw output against set-semantics oracle per checkpoint."
    )
    p.add_argument("--crown-exec", required=True, help="Path to crown_exec")
    p.add_argument("--stream-crown", required=True, help="Path to +|-|* stream")
    p.add_argument("--k", type=int, required=True)
    p.add_argument("--a", type=int, required=True)
    p.add_argument("--b", type=int, required=True)
    p.add_argument("--root-kind", default="auto")
    p.add_argument("--root-pos", type=int, default=0)
    p.add_argument(
        "--max-phases",
        type=int,
        default=0,
        help="Limit checked checkpoints; 0 means all milestones.",
    )
    return p.parse_args()


def build_fwd(rels: dict[int, set[tuple[int, int]]], k: int) -> dict[int, dict[int, list[int]]]:
    out: dict[int, dict[int, list[int]]] = {}
    for rid in range(1, k + 1):
        m: dict[int, list[int]] = defaultdict(list)
        for u, v in rels[rid]:
            m[u].append(v)
        out[rid] = m
    return out


def left_reachable_xa(rels: dict[int, set[tuple[int, int]]], k: int, a: int) -> set[int] | None:
    if a == 1:
        return None
    fwd = build_fwd(rels, k)
    dp: set[int] = set()
    for _, v in rels[1]:
        dp.add(v)
    for rid in range(2, a):
        ndp: set[int] = set()
        fm = fwd[rid]
        for src in dp:
            for dst in fm.get(src, []):
                ndp.add(dst)
        dp = ndp
        if not dp:
            break
    return dp


def right_reachable_xb(rels: dict[int, set[tuple[int, int]]], k: int, b: int) -> set[int] | None:
    if b == k + 1:
        return None
    fwd = build_fwd(rels, k)
    reachable: set[int] = set()
    for src, _ in rels[k]:
        reachable.add(src)
    for rid in range(k - 1, b - 1, -1):
        ndp: set[int] = set()
        fm = fwd[rid]
        for src, dsts in fm.items():
            if any(dst in reachable for dst in dsts):
                ndp.add(src)
        reachable = ndp
        if not reachable:
            break
    return reachable


def oracle_raw_keys(rels: dict[int, set[tuple[int, int]]], k: int, a: int, b: int) -> set[str]:
    out: set[str] = set()
    if a > b:
        return out

    fwd = build_fwd(rels, k)
    left_ok = left_reachable_xa(rels, k, a)
    right_ok = right_reachable_xb(rels, k, b)

    def left_allowed(xa: int) -> bool:
        return left_ok is None or xa in left_ok

    def right_allowed(xb: int) -> bool:
        return right_ok is None or xb in right_ok

    if a == b:
        if a == 1:
            if right_ok is None:
                return out
            for v in sorted(right_ok):
                out.add(str(v))
            return out

        if a == k + 1:
            if left_ok is None:
                return out
            for v in sorted(left_ok):
                out.add(str(v))
            return out

        candidates: set[int] = set()
        if left_ok is not None:
            candidates |= left_ok
        if right_ok is not None:
            candidates |= right_ok

        for x in candidates:
            if left_allowed(x) and right_allowed(x):
                out.add(str(x))
        return out

    def emit(values: list[int]) -> None:
        if not left_allowed(values[0]):
            return
        if not right_allowed(values[-1]):
            return
        out.add("|".join(str(v) for v in values))

    def dfs(rid: int, prev: int, values: list[int]) -> None:
        if rid > b - 1:
            emit(values)
            return
        for nxt in fwd[rid].get(prev, []):
            values.append(nxt)
            dfs(rid + 1, nxt, values)
            values.pop()

    for u, v in rels[a]:
        values = [u, v]
        dfs(a + 1, v, values)

    return out


def run_delta_raw_folded_keys(
    crown_exec: Path,
    stream_prefix_lines: list[str],
    k: int,
    a: int,
    b: int,
    root_kind: str,
    root_pos: int,
) -> set[str]:
    with tempfile.NamedTemporaryFile(mode="w", suffix=".crown", delete=False) as tmp:
        for line in stream_prefix_lines:
            tmp.write(line)
            if not line.endswith("\n"):
                tmp.write("\n")
        temp_path = Path(tmp.name)

    cmd = [
        str(crown_exec),
        "--query",
        "linek_count",
        "--input",
        str(temp_path),
        "--k",
        str(k),
        "--a",
        str(a),
        "--b",
        str(b),
        "--linek-mode",
        "raw",
        "--mode",
        "delta",
        "--root-kind",
        root_kind,
        "--root-pos",
        str(root_pos),
        "--print-result",
    ]

    proc = subprocess.run(cmd, capture_output=True, text=True)
    temp_path.unlink(missing_ok=True)
    if proc.returncode != 0:
        raise RuntimeError(proc.stdout + "\n" + proc.stderr)

    live: set[str] = set()
    for raw in proc.stdout.splitlines():
        line = raw.strip()
        m = DELTA_RE.match(line)
        if not m:
            continue
        sign, key = m.group(1), m.group(2)
        if sign == "+":
            live.add(key)
        else:
            live.discard(key)
    return live


def main() -> None:
    args = parse_args()
    crown_exec = Path(args.crown_exec).resolve()
    stream_path = Path(args.stream_crown).resolve()

    if not crown_exec.exists():
        raise FileNotFoundError(f"crown_exec not found: {crown_exec}")
    if not stream_path.exists():
        raise FileNotFoundError(f"stream not found: {stream_path}")

    all_lines = [ln.rstrip("\n") for ln in stream_path.read_text(encoding="utf-8").splitlines() if ln.strip()]
    milestone_indices = [i for i, ln in enumerate(all_lines) if ln == "*"]
    if not milestone_indices:
        raise ValueError("No milestones found in stream")

    rels: dict[int, set[tuple[int, int]]] = {rid: set() for rid in range(1, args.k + 1)}
    oracle_phase_keys: list[set[str]] = []
    for line in all_lines:
        if line == "*":
            oracle_phase_keys.append(oracle_raw_keys(rels, args.k, args.a, args.b))
            continue
        op, rid_s, src_s, dst_s = line.split("|")
        rid = int(rid_s)
        tup = (int(src_s), int(dst_s))
        if op == "+":
            rels[rid].add(tup)
        elif op == "-":
            rels[rid].discard(tup)
        else:
            raise ValueError(f"invalid op line: {line}")

    phases = len(milestone_indices)
    if args.max_phases > 0:
        phases = min(phases, args.max_phases)

    mismatches = 0
    for p in range(phases):
        prefix_end = milestone_indices[p]
        prefix_lines = all_lines[: prefix_end + 1]
        dynagox_keys = run_delta_raw_folded_keys(
            crown_exec,
            prefix_lines,
            args.k,
            args.a,
            args.b,
            args.root_kind,
            args.root_pos,
        )
        oracle_keys = oracle_phase_keys[p]

        if dynagox_keys != oracle_keys:
            mismatches += 1
            only_dynagox = sorted(dynagox_keys - oracle_keys)
            only_oracle = sorted(oracle_keys - dynagox_keys)
            print(f"[phase {p}] MISMATCH")
            print(f"  dynagox_only={len(only_dynagox)} oracle_only={len(only_oracle)}")
            if only_dynagox:
                print(f"  sample dynagox_only: {only_dynagox[:5]}")
            if only_oracle:
                print(f"  sample oracle_only: {only_oracle[:5]}")
        else:
            print(f"[phase {p}] OK rows={len(dynagox_keys)}")

    if mismatches:
        raise SystemExit(f"Validation failed: {mismatches}/{phases} phases mismatched")

    print(f"Validation passed: {phases}/{phases} phases matched")


if __name__ == "__main__":
    main()
