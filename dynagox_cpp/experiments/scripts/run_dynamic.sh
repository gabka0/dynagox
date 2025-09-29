#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BIN_PATH="${IMPL_PATH}/build/experiments"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_dynamic.log"
touch "${LOG_FILE}"

queries=("line5c")
for query in "${queries[@]}"; do
  echo "Running ${query} window..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/graph/window/graph.csv" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query} window did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done

queries=("line5gc")
for query in "${queries[@]}"; do
  echo "Running ${query}[F] window..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/graph/window/graph.csv" -m "full" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[F] window did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  echo "Running ${query}[D] window..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/graph/window/graph.csv" -m "delta" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[D] window did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done

queries=("tpch3")
for query in "${queries[@]}"; do
  echo "Running ${query}[F] window..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/tpch/window/${query}_all.csv" -m "full" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[F] window did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  echo "Running ${query}[D] window..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/tpch/window/${query}_all.csv" -m "delta" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[D] window did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  echo "Running ${query}[F] dynamic..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/tpch/dynamic/${query}_all.csv" -m "full" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[F] dynamic did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  echo "Running ${query}[D] dynamic..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${BIN_PATH}/dynagox_exec" -q "${query}" \
    -i "${EXPERIMENT_DATA}/tpch/dynamic/${query}_all.csv" -m "delta" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[D] dynamic did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
