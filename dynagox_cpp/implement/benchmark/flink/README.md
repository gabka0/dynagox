Flink Baseline Template
=======================

Purpose
-------
This folder provides a baseline template for Apache Flink comparison against DynaGox `linek_count`.

What is included
----------------
- `linek_count_template.sql`: SQL template for insert-only baseline on Line queries.

Input format
------------
Use `prepare_snap_stream.py` to produce a Flink CSV stream:

`seq,op,rid,src,dst,is_milestone`

Notes
-----
1. The SQL template is insert-only (`op='+'`) to establish baseline correctness/perf first.
2. Full delete-aware baseline needs retract/upsert pipeline (DataStream or changelog connector).
3. For fair performance comparison, keep the same `(dataset,k,a,b,seed,update_count,milestone_every)` config.

Suggested workflow
------------------
1. Generate stream:
   - `python dynagox_cpp/implement/benchmark/prepare_snap_stream.py ...`
2. Run DynaGox benchmark on same config.
3. Run Flink SQL query for the matching `(k,a,b)`.
4. Export Flink run metrics/signatures to CSV with schema:
   - `system,dataset,k,a,b,phase,phase_idx,ops,time_ms,throughput,hash,rows`
5. Use `compare_linek_systems.py` to validate and plot.
