#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/hpps"
cd "${BACKEND_PATH}"

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  echo "Compiling ${query}..."
  rm -f "${EXPERIMENTS_PATH}/hpps/${query}.hpp"
  sbt "toast -l cpp -O3 -o ${EXPERIMENTS_PATH}/hpps/${query}.hpp ${EXPERIMENTS_PATH}/queries/ceb/${query}.sql"
done

echo "Done."
