#!/usr/bin/env python3
import argparse
import csv
import random


def load_edges(path):
    edges = []
    seen = set()
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            s = line.strip()
            if not s or s.startswith("#"):
                continue
            parts = s.split()
            if len(parts) < 2:
                continue
            u = int(parts[0])
            v = int(parts[1])
            e = (u, v)
            if e not in seen:
                seen.add(e)
                edges.append(e)
    return edges


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--dataset", required=True)
    p.add_argument("--k", type=int, required=True)
    p.add_argument("--seed", type=int, default=42)
    p.add_argument("--update-count", type=int, default=100000)
    p.add_argument("--milestone-every", type=int, default=10000)
    p.add_argument("--initial-ratio", type=float, default=0.5)
    p.add_argument(
        "--mode",
        choices=["full_updates", "insertion_only_set"],
        default="full_updates",
    )
    p.add_argument("--out-crown", required=True)
    p.add_argument("--out-flink", required=True)
    args = p.parse_args()

    edges = load_edges(args.dataset)
    if not edges:
        raise SystemExit("No edges found")

    rng = random.Random(args.seed)
    rng.shuffle(edges)

    initial_count = min(len(edges), int(args.update_count * args.initial_ratio))
    if initial_count < 1:
        initial_count = min(len(edges), 1)

    # Track active relation tuples as (rid, src, dst) to preserve
    # relation-specific set semantics under updates.
    active = set()
    active_list = []

    def add_active(t):
        if t not in active:
            active.add(t)
            active_list.append(t)

    def remove_active(t):
        if t in active:
            active.remove(t)
            # keep O(n) simple here; benchmark generation is offline
            try:
                active_list.remove(t)
            except ValueError:
                pass

    rows_flink = []
    seq = 0

    with open(args.out_crown, "w", encoding="utf-8") as fc:
        for i in range(initial_count):
            rid = rng.randint(1, args.k)
            u, v = edges[i]
            fc.write(f"+|{rid}|{u}|{v}\n")
            rows_flink.append((seq, "+", rid, u, v, 0))
            seq += 1
            add_active((rid, u, v))
        fc.write("*\n")
        rows_flink.append((seq, "M", 0, 0, 0, 1))
        seq += 1

        ops_in_window = 0
        for _ in range(args.update_count):
            rid = rng.randint(1, args.k)
            do_insert = args.mode == "insertion_only_set" or (rng.random() < 0.5)
            if not active_list:
                do_insert = True
            elif len(active_list) == len(edges) * args.k:
                do_insert = False

            if do_insert:
                chosen_edge = None
                for _ in range(8):
                    cand = edges[rng.randrange(len(edges))]
                    if (rid, cand[0], cand[1]) not in active:
                        chosen_edge = cand
                        break
                if chosen_edge is None:
                    for cand in edges:
                        if (rid, cand[0], cand[1]) not in active:
                            chosen_edge = cand
                            break
                if chosen_edge is None:
                    continue
                u, v = chosen_edge
                fc.write(f"+|{rid}|{u}|{v}\n")
                rows_flink.append((seq, "+", rid, u, v, 0))
                seq += 1
                add_active((rid, u, v))
            else:
                idx = rng.randrange(len(active_list))
                chosen = active_list[idx]
                rr, u, v = chosen
                fc.write(f"-|{rr}|{u}|{v}\n")
                # Delete exactly the active relation tuple.
                rows_flink.append((seq, "-", rr, u, v, 0))
                seq += 1
                remove_active(chosen)

            ops_in_window += 1
            if ops_in_window == args.milestone_every:
                fc.write("*\n")
                rows_flink.append((seq, "M", 0, 0, 0, 1))
                seq += 1
                ops_in_window = 0

        if ops_in_window != 0:
            fc.write("*\n")
            rows_flink.append((seq, "M", 0, 0, 0, 1))

    with open(args.out_flink, "w", newline="", encoding="utf-8") as ff:
        writer = csv.writer(ff)
        writer.writerow(["seq", "op", "rid", "src", "dst", "is_milestone"])
        writer.writerows(rows_flink)


if __name__ == "__main__":
    main()
