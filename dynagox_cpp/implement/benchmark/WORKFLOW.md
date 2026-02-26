Line(k,a,b) Benchmark Workflow
==============================

1. Build benchmark executable
-----------------------------
```powershell
cmake --build dynagox_cpp/implement/build --target bench_linek_count -j 8
```

2. Prepare deterministic stream from SNAP
-----------------------------------------
```powershell
python dynagox_cpp/implement/benchmark/prepare_snap_stream.py `
  --dataset path\to\snap.txt `
  --k 6 `
  --seed 42 `
  --update-count 200000 `
  --milestone-every 20000 `
  --initial-ratio 0.5 `
  --out-crown dynagox_cpp/implement/benchmark/stream.crown `
  --out-flink dynagox_cpp/implement/benchmark/stream.flink.csv
```

3. Run DynaGox benchmark grid (k<=6, b-a<=4)
---------------------------------------------
```powershell
.\dynagox_cpp\implement\run_linek_bench_grid.ps1 `
  -Dataset path\to\snap.txt `
  -OutCsv dynagox_cpp/implement/benchmark/dynagox_results.csv `
  -Seed 42 -UpdateCount 200000 -MilestoneEvery 20000 -InitialRatio 0.5
```

4. Run Flink baseline
---------------------
1. Use `benchmark/flink/linek_count_template.sql`.
2. Run matching `(k,a,b)` queries in Flink on `stream.flink.csv`.
3. Export Flink results to CSV with columns:
   - `system,dataset,k,a,b,phase,phase_idx,ops,time_ms,throughput,hash,rows`

5. Compare and plot
-------------------
```powershell
python dynagox_cpp/implement/benchmark/compare_linek_systems.py `
  --dynagox-csv dynagox_cpp/implement/benchmark/dynagox_results.csv `
  --flink-csv dynagox_cpp/implement/benchmark/flink_results.csv `
  --out-dir dynagox_cpp/implement/benchmark/report
```

Outputs:
- `correctness_report.csv`
- `throughput.png`
- `latency_ms.png`
- `summary.txt`
