#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BIN_PATH="${IMPL_PATH}/build/experiments"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_degenerate.log"
touch "${LOG_FILE}"

echo "Running line5sd..." >> "${LOG_FILE}"
timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/crown_exec" -q "line5sd" \
  -i "${EXPERIMENT_DATA}/graph/degenerate/graph.csv" >> "${LOG_FILE}" 2>&1
if [ $? -eq 137 ]; then
  echo "Timeout: line5sd did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
fi
echo "" >> "${LOG_FILE}"
