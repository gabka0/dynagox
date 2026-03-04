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
Use the runner script to export Flink CSV in the same schema:

```powershell
python dynagox_cpp/implement/benchmark/run_flink_linek_set_updates.py `
  --flink-sql-client C:\path\to\flink\bin\sql-client.sh `
  --stream-csv dynagox_cpp/implement/benchmark/stream.flink.csv `
  --dataset-label path\to\snap.txt `
  --k-min 2 --k-max 6 --max-span 4 `
  --timing-mode strict_per_phase `
  --out-csv dynagox_cpp/implement/benchmark/flink_results.csv
```

Timing modes:
- `strict_per_phase`: one Flink run per checkpoint (recommended for fair timing).
- `split_total`: one Flink run per `(k,a,b)`, total time split across checkpoints (fast but approximate).

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
