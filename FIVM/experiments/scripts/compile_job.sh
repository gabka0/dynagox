#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/m3s"
mkdir -p "${EXPERIMENTS_PATH}/hpps"

queries=("job8" "job16" "job19" "job8s" "job8m1" "job8m2")
for query in "${queries[@]}"; do
  echo "Compiling ${query}..."
  rm -f "${EXPERIMENTS_PATH}/m3s/${query}.m3"
  rm -f "${EXPERIMENTS_PATH}/hpps/${query}.hpp"
  "${BIN_PATH}/run_frontend.sh" --single "${EXPERIMENTS_PATH}/queries/job/${query}.sql" -o "${EXPERIMENTS_PATH}/m3s/${query}.m3"
  "${BIN_PATH}/run_backend.sh" "${EXPERIMENTS_PATH}/m3s/${query}.m3" -o "${EXPERIMENTS_PATH}/hpps/${query}.hpp"
done

echo "Done."
