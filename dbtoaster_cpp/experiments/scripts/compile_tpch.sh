#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/hpps"
cd "${BACKEND_PATH}"

queries=("tpch3" "tpch5" "tpch8" "tpch9" "tpch10")
for query in "${queries[@]}"; do
  echo "Compiling ${query}..."
  rm -f "${EXPERIMENTS_PATH}/hpps/${query}.hpp"
  sbt "toast -l cpp -O3 -o ${EXPERIMENTS_PATH}/hpps/${query}.hpp ${EXPERIMENTS_PATH}/queries/tpch/${query}.sql"
done

echo "Done."
