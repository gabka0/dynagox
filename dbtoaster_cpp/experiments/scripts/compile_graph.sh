#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/hpps"
cd "${BACKEND_PATH}"

queries=("line5c" "line5s" "line5cd" "line5gc")
for query in "${queries[@]}"; do
  echo "Compiling ${query}..."
  rm -f "${EXPERIMENTS_PATH}/hpps/${query}.hpp"
  sbt "toast -l cpp -O3 -o ${EXPERIMENTS_PATH}/hpps/${query}.hpp ${EXPERIMENTS_PATH}/queries/line5/${query}.sql"
done

echo "Compiling dumbbell..."
rm -f "${EXPERIMENTS_PATH}/hpps/dumbbell.hpp"
sbt "toast -l cpp -O3 -o ${EXPERIMENTS_PATH}/hpps/dumbbell.hpp ${EXPERIMENTS_PATH}/queries/dumbbell/dumbbell.sql"

echo "Done."
