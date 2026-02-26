# SNAP Benchmark Report (Wiki-Vote, Set Semantics)

## 1) Objective

Per Prof. Qichen Wang's request (Feb 26, 2026), run SNAP benchmarks with:

- `k <= 6`
- `b - a <= 4`
- comparison against Flink for correctness and efficiency

and prepare for delta-enumeration follow-up.

## 2) Dataset and Workload

- Dataset: `wiki-Vote.txt` (from `wiki-Vote.txt.gz`)
- Label used in CSV: `/home/kassym/Downloads/wiki-Vote.txt`
- Stream generation: `seed=42`, `update_count=5000`, `milestone_every=500`, `initial_ratio=0.5`
- Tested `k`: `{2,3,4,5,6}`
- Total `(k,a,b)` combinations with `b-a<=4`: `76`
- Checkpoints per combination: `11` (`1` initial + `10` update windows)
- Total measured rows per system: `76 * 11 = 836`

## 3) Alignment and Methodology

The pipeline was aligned to **set semantics with effective updates**:

1. Stream generation tracks active tuples as `(rid, src, dst)` so relation-level updates are consistent.
2. Deletes remove the exact active tuple `(rid, src, dst)`.
3. Flink baseline is delete-aware and reconstructs relation states with signed counts (`+1/-1`) and `HAVING SUM(...) > 0`.
4. An independent set-semantics oracle was used as a third reference.

This avoids false mismatches caused by insertion-only SQL or relation-unaware stream generation.

## 4) Artifacts

- DynaGox CSV: `dynagox_cpp/implement/benchmark/set_semantics_full/dynagox.csv`
- Flink CSV: `dynagox_cpp/implement/benchmark/set_semantics_full/flink.csv`
- Oracle CSV: `dynagox_cpp/implement/benchmark/set_semantics_full/oracle.csv`
- Correctness (DynaGox vs Flink): `dynagox_cpp/implement/benchmark/set_semantics_full/report_dynagox_vs_flink/correctness_report.csv`
- Correctness (DynaGox vs Oracle): `dynagox_cpp/implement/benchmark/set_semantics_full/report_dynagox_vs_oracle/correctness_report.csv`
- Flink throughput plot: `dynagox_cpp/implement/benchmark/set_semantics_full/report_dynagox_vs_flink/throughput.png`
- Flink latency plot: `dynagox_cpp/implement/benchmark/set_semantics_full/report_dynagox_vs_flink/latency_ms.png`
- Oracle throughput plot: `dynagox_cpp/implement/benchmark/set_semantics_full/report_dynagox_vs_oracle/throughput.png`
- Oracle latency plot: `dynagox_cpp/implement/benchmark/set_semantics_full/report_dynagox_vs_oracle/latency_ms.png`

## 5) Correctness Results

From both summary files:

- rows compared: `836`
- exact match rows: `836`
- correctness rate: `1.0000`

So for all tested `(k,a,b,phase_idx)`, DynaGox hash and row counts match both Oracle and Flink.

## 6) Efficiency Snapshot

Aggregated throughput (all combinations):

- DynaGox initial-load phase: `1,612,629 ops/s`
- Flink initial-load phase: `2,776 ops/s`
- DynaGox updates phase: `1,798,791 ops/s`
- Flink updates phase: `555 ops/s`

Throughput speedup (`dynagox/flink`) on update checkpoints:

- min: `917.4x`
- p50: `3564.9x`
- p90: `4374.5x`
- max: `4979.4x`

## 7) Important Timing Caveat

Current Flink script computes all checkpoints for one `(k,a,b)` in one batch SQL job, then splits total elapsed time evenly across checkpoints.  
So correctness is fully valid, but per-checkpoint latency/throughput for Flink is an approximation.

For publication-grade timing fairness, next revision should measure each checkpoint with direct incremental timing (or run per-window jobs and include startup policy explicitly).

## 8) Conclusion

After semantic alignment, the benchmark confirms:

1. **Correctness:** DynaGox is consistent with both Oracle and Flink under set semantics (`836/836` exact matches).
2. **Performance trend:** DynaGox is substantially faster than current Flink baseline on this setup.
3. **Next technical step:** implement and benchmark **delta enumeration** as requested.

## 9) Proposed Next Steps

1. Add strict checkpoint timing mode for Flink (non-uniform per checkpoint measurement).
2. Add delta stream output for DynaGox (`+/-` result deltas per update).
3. Add matching Flink changelog delta baseline.
4. Compare full enumeration vs delta enumeration on latency and throughput.
