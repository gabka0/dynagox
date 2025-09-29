#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_all.log"
touch "${LOG_FILE}"

NUM_OF_RUNS=5

tasks=("lsqb" "tpcds" "progress")
for task in "${tasks[@]}"; do
  echo "Running ${task}..." >> "${LOG_FILE}"
  CURRENT_RUN=1
  while [[ ${CURRENT_RUN} -le ${NUM_OF_RUNS} ]]; do
    bash "${CURRENT_PATH}/run_${task}.sh"
    CURRENT_RUN=$((CURRENT_RUN + 1))
  done
done

echo "Done." >> "${LOG_FILE}"
