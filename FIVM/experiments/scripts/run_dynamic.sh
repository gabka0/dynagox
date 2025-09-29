#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BIN_PATH="${EXPERIMENTS_PATH}/bin"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_dynamic.log"
touch "${LOG_FILE}"

queries=("line5c_w" "line5gc_w")
for query in "${queries[@]}"; do
  echo "Running ${query} window..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/${query}" \
    -i "${EXPERIMENT_DATA}/graph/window/graph.csv" --no-output >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query} window did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
