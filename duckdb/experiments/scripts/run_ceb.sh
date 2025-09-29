#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_ceb.log"
touch "${LOG_FILE}"

MAIN_CLASS="duckdb.experiments.Main"
JAR_FILE="${TARGET_PATH}/duckdb-1.0-SNAPSHOT-jar-with-dependencies.jar"

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  echo "Running ${query}..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA}/ceb/x10/${query}_all.csv" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query} did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
