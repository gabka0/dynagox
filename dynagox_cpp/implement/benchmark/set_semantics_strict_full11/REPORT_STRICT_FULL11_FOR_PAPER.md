# SNAP Benchmark Report (Wiki-Vote, Strict Full Checkpoints)

## 1) Objective

Run a publication-grade strict benchmark on SNAP wiki-Vote with:

- `k <= 6`
- `b - a <= 4`
- delete-aware set semantics (`+/-` stream)
- checkpoint-level Flink timing (no split-time approximation)

## 2) Workload and Checkpoints

- Dataset: `/home/kassym/Downloads/wiki-Vote.txt`
- Stream params: `seed=42`, `update_count=5000`, `milestone_every=500`, `initial_ratio=0.5`
- Checkpoints per `(k,a,b)`: `11` (`1` initial-load + `10` updates)
- Total `(k,a,b)` combos for `k=2..6`, `b-a<=4`: `76`
- Total rows per system: `76 * 11 = 836`

## 3) Methodology

1. Generate deterministic relation-aware update streams (`mode=full_updates`).
2. Run DynaGox benchmark on the same stream checkpoints.
3. Run Flink in `strict_per_phase` mode (one Flink job per checkpoint).
4. Validate both with hash and row-count alignment by `(k,a,b,phase,phase_idx)`.
5. Cross-check DynaGox with independent oracle.

## 4) Strict Timing Implementation Note

`run_flink_linek_set_updates.py` was hardened for Flink 2.2 SQL client behavior:

- parse `Job ID` from SQL-client output when available
- fallback to REST job-list delta when output format omits job id
- wait for terminal state before sink collection when job id is known

This avoids false failures from client-output format differences.

## 5) Main Correctness Results

- DynaGox vs Flink:
  - rows compared: `836`
  - hash mismatches: `0`
  - row-count mismatches: `0`
- DynaGox vs Oracle:
  - rows compared: `836`
  - hash mismatches: `0`
  - row-count mismatches: `0`

So correctness is fully aligned on all strict checkpoints.

## 6) Strict Timing Results (Full 11-Checkpoint Grid)

From `report_manual/summary.txt`:

- DynaGox average `time_ms`:
  - initial-load: `1.523 ms`
  - updates: `0.271 ms`
- Flink average `time_ms`:
  - initial-load: `9132.001 ms`
  - updates: `9141.862 ms`

Throughput speedup (`dynagox/flink`, aligned rows):

- min: `3348.617x`
- p50: `35500.720x`
- p90: `42540.955x`
- max: `48800.841x`

Phase medians:

- initial-load speedup p50: `6050.927x`
- updates speedup p50: `36232.771x`

By `k`, speedup median:

- `k=2`: `18594.5x`
- `k=3`: `31405.2x`
- `k=4`: `34846.0x`
- `k=5`: `35936.0x`
- `k=6`: `38575.8x`

## 7) Full Strict Runtime (Wall-Clock)

Full strict Flink pass for `k=2..6` completed in approximately:

- start: `2026-03-04 18:36:06`
- end: `2026-03-04 20:43:36`
- elapsed: `~2h 07m 30s`

Per-k strict runtime (same session):

- `k=2`: `~9m 08s`
- `k=3`: `~15m 57s`
- `k=4`: `~24m 45s`
- `k=5`: `~34m 23s`
- `k=6`: `~43m 17s`

## 8) Artifacts

- DynaGox CSV: `set_semantics_strict_full11/dynagox.csv`
- Flink CSV (strict full): `set_semantics_strict_full11/flink.csv`
- Oracle CSV: `set_semantics_strict_full11/oracle.csv`
- Manual metrics summary: `set_semantics_strict_full11/report_manual/summary.txt`
- DynaGox vs Flink report: `set_semantics_strict_full11/report_dynagox_vs_flink/`
- DynaGox vs Oracle report: `set_semantics_strict_full11/report_dynagox_vs_oracle/`

## 9) Conclusion

For the full strict 11-checkpoint benchmark on wiki-Vote (`k<=6`, `b-a<=4`):

1. Correctness is exact across all checkpoints (DynaGox matches both Flink and Oracle).
2. DynaGox is substantially faster than the current Flink strict baseline in this setup.
3. This run is suitable as the strict timing/correctness baseline section for paper text before delta-enumeration experiments.
