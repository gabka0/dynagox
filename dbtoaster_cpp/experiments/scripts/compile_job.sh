#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/hpps"
cd "${BACKEND_PATH}"

queries=("job8" "job16" "job19" "job8s")
for query in "${queries[@]}"; do
  echo "Compiling ${query}..."
  rm -f "${EXPERIMENTS_PATH}/hpps/${query}.hpp"
  sbt "toast -l cpp -O3 -o ${EXPERIMENTS_PATH}/hpps/${query}.hpp ${EXPERIMENTS_PATH}/queries/job/${query}.sql"
done

echo "Done."
