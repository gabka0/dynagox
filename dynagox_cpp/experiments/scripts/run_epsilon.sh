#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BIN_PATH="${IMPL_PATH}/build/experiments"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_epsilon.log"
touch "${LOG_FILE}"

epsilons=("0.01" "0.02" "0.05" "0.1" "0.2")
for epsilon in "${epsilons[@]}"; do
  echo "Running line5c[e=${epsilon}]..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "line5c" \
    -i "${EXPERIMENT_DATA}/graph/insert/graph.csv" -e "${epsilon}" -p >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: line5c[e=${epsilon}] did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
