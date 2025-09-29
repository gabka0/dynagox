#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_graph.log"
touch "${LOG_FILE}"

MAIN_CLASS="dynagox.experiments.Main"

queries=("line5c" "line5s" "line5m" "line5cd" "dumbbell")
for query in "${queries[@]}"; do
  echo "Running ${query}..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${JAVA11_HOME}/bin/java" \
    -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA}/graph/insert/graph.csv" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query} did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done

queries=("line5gc")
for query in "${queries[@]}"; do
  echo "Running ${query}[F]..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${JAVA11_HOME}/bin/java" \
    -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA}/graph/insert/graph.csv" -m "full" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[F] did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  echo "Running ${query}[D]..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${JAVA11_HOME}/bin/java" \
    -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA}/graph/insert/graph.csv" -m "delta" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query}[D] did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
