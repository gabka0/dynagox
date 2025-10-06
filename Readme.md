## Repository for Paper: Approximate Query Processing under Updates
This repository contains the code and resources for our paper: Approximate Query Processing under Updates.

### Queries
The SQL queries used in the experiments can be found in [Queries](Queries.md).

### Project Structure
The repository is organized as follows:
   ```
    ├── data/                # Scripts for preparing input data
    ├── dynagox_cpp/         # DynaGox(C++) implementation and related scripts
    ├── dynagox_flink/       # DynaGox(Flink) implementation and related scripts
    ├── crown_cpp/           # Crown(C++) implementation and related scripts
    ├── dbtoaster_cpp/       # DBToaster (from https://github.com/dbtoaster) and related scripts
    ├── FIVM/                # F-IVM (from https://github.com/fdbresearch/FIVM) and related scripts
    ├── feldera/             # feldera (from https://www.feldera.com) and related scripts
    ├── duckdb/              # DuckDB and related scripts
    ├── ResultVerifier/      # Utilities for verifying the correctness of experiment results
    ├── Queries.md           # List of queries used in the project
    └── README.md            # This file
   ```
The `MultiHashMap` implementation in DynaGox is inspired by the [F-IVM](https://github.com/fdbresearch/FIVM) project, which is licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

### Steps to Run Experiments

1. Prepare Data
   ```shell
   bash data/scripts/prepare_graph.sh
   bash data/scripts/prepare_ldbc.sh
   bash data/scripts/prepare_imdb.sh
   bash data/scripts/prepare_tpch.sh
   bash data/scripts/prepare_tpcds.sh
   ```
2. Build DynaGox
   ```shell
   bash dynagox_cpp/experiments/compile_all.sh
   ```
3. Convert Data
   ```shell
   ./dynagox_cpp/implement/build/convert/convert_graph -i ./data/graph/epinions.csv -o /path/to/data/dynagox/graph/insert/
   ./dynagox_cpp/implement/build/convert/convert_graph -i ./data/graph/epinions.csv -o /path/to/data/dynagox/graph/window/ -w
   ./dynagox_cpp/implement/build/convert/convert_graph -i ./data/graph/epinions.csv -o /path/to/data/dynagox/graph/progress/ -p
   ./dynagox_cpp/implement/build/convert/convert_graph -i ./data/graph/epinions.csv -o /path/to/data/dynagox/graph/degenerate/ -d
   
   ./dynagox_cpp/implement/build/convert/convert_lsqb -i ./data/ldbc/ -o /path/to/data/dynagox/lsqb/sf30/
   
   ./dynagox_cpp/implement/build/convert/convert_job -i ./data/imdb/ -o /path/to/data/dynagox/job/x1/ -q 19 -f 1
   ./dynagox_cpp/implement/build/convert/convert_job -i ./data/imdb/ -o /path/to/data/dynagox/job/x2/ -q 19 -f 2
   ./dynagox_cpp/implement/build/convert/convert_job -i ./data/imdb/ -o /path/to/data/dynagox/job/x5/ -q 19 -f 5
   ./dynagox_cpp/implement/build/convert/convert_job -i ./data/imdb/ -o /path/to/data/dynagox/job/x10/
   ./dynagox_cpp/implement/build/convert/convert_job -i ./data/imdb/ -o /path/to/data/dynagox/job/x20/ -q 19 -f 20
   
   ./dynagox_cpp/implement/build/convert/convert_ceb -i ./data/imdb/ -o /path/to/data/dynagox/ceb/x10/
   
   ./dynagox_cpp/implement/build/convert/convert_tpch -i ./data/tpch/ -o /path/to/data/dynagox/tpch/insert/
   ./dynagox_cpp/implement/build/convert/convert_tpch -i ./data/tpch/ -o /path/to/data/dynagox/tpch/window/ -w
   ./dynagox_cpp/implement/build/convert/convert_tpch -i ./data/tpch/ -o /path/to/data/dynagox/tpch/dynamic/ -d
   
   ./dynagox_cpp/implement/build/convert/convert_tpcds -i ./data/tpcds/ -o /path/to/data/dynagox/tpcds/insert/
   ```
4. Build Other Systems
   ```shell
   bash FIVM/experiments/scripts/compile_all.sh
   bash FIVM/experiments/scripts/build_all.sh
   
   bash dbtoaster_cpp/experiments/scripts/compile_all.sh
   bash dbtoaster_cpp/experiments/scripts/build_all.sh
   
   bash dynagox_flink/experiments/scripts/compile_all.sh
   
   bash feldera/experiments/scripts/compile_client.sh
   
   bash duckdb/experiments/scripts/compile_all.sh
   ```
5. Run Experiments
   ```shell
   bash ${SYSTEM_NAME}/experiments/scripts/run_all.sh
   # Results will be saved in ${SYSTEM_NAME}/experiments/log/
   ```
