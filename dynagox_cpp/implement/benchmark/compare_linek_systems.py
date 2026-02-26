#!/usr/bin/env python3
import argparse
import os
import pandas as pd
import matplotlib.pyplot as plt


KEY_COLS = ["dataset", "k", "a", "b", "phase", "phase_idx"]


def load_csv(path: str, default_system: str) -> pd.DataFrame:
    df = pd.read_csv(path)
    if "system" not in df.columns:
        df["system"] = default_system
    required = KEY_COLS + ["ops", "time_ms", "throughput", "hash", "rows", "system"]
    missing = [c for c in required if c not in df.columns]
    if missing:
        raise ValueError(f"{path} missing columns: {missing}")
    return df


def check_correctness(df_d: pd.DataFrame, df_f: pd.DataFrame) -> pd.DataFrame:
    left = df_d[KEY_COLS + ["hash", "rows"]].rename(
        columns={"hash": "hash_dynagox", "rows": "rows_dynagox"}
    )
    right = df_f[KEY_COLS + ["hash", "rows"]].rename(
        columns={"hash": "hash_flink", "rows": "rows_flink"}
    )
    merged = left.merge(right, on=KEY_COLS, how="outer", indicator=True)
    merged["hash_match"] = merged["hash_dynagox"] == merged["hash_flink"]
    merged["rows_match"] = merged["rows_dynagox"] == merged["rows_flink"]
    merged["ok"] = (
        (merged["_merge"] == "both")
        & merged["hash_match"].fillna(False)
        & merged["rows_match"].fillna(False)
    )
    return merged


def plot_metric(df: pd.DataFrame, metric: str, out_png: str):
    plt.figure(figsize=(12, 6))
    grouped = df.groupby(["system", "dataset", "k"], as_index=False)[metric].mean()
    for (system, k), g in grouped.groupby(["system", "k"]):
        x = g["dataset"].astype(str)
        y = g[metric]
        label = f"{system}-k{k}"
        plt.plot(x, y, marker="o", label=label)
    plt.xticks(rotation=30, ha="right")
    plt.ylabel(metric)
    plt.xlabel("dataset")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=160)
    plt.close()


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--dynagox-csv", required=True)
    p.add_argument("--flink-csv", required=True)
    p.add_argument("--out-dir", required=True)
    args = p.parse_args()

    os.makedirs(args.out_dir, exist_ok=True)

    d = load_csv(args.dynagox_csv, "dynagox")
    f = load_csv(args.flink_csv, "flink")

    corr = check_correctness(d, f)
    corr.to_csv(os.path.join(args.out_dir, "correctness_report.csv"), index=False)

    all_df = pd.concat([d, f], ignore_index=True)
    plot_metric(all_df, "throughput", os.path.join(args.out_dir, "throughput.png"))
    plot_metric(all_df, "time_ms", os.path.join(args.out_dir, "latency_ms.png"))

    ok_rate = corr["ok"].mean() if len(corr) else 0.0
    with open(os.path.join(args.out_dir, "summary.txt"), "w", encoding="utf-8") as out:
        out.write(f"rows_compared={len(corr)}\n")
        out.write(f"correctness_ok_rate={ok_rate:.4f}\n")
        out.write(f"exact_ok_rows={(corr['ok']).sum() if len(corr) else 0}\n")


if __name__ == "__main__":
    main()
