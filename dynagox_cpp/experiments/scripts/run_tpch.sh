#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BIN_PATH="${IMPL_PATH}/build/experiments"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_tpch.log"
touch "${LOG_FILE}"

queries=("tpch3" "tpch5" "tpch8" "tpch9" "tpch10")
for query in "${queries[@]}"; do
  echo "Running ${query}[F]..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/tpch/insert/${query}_all.csv" -m "full" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[F] did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  echo "Running ${query}[D]..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/tpch/insert/${query}_all.csv" -m "delta" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[D] did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
