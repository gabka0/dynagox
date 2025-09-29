#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
QUERIES_PATH="${EXPERIMENTS_PATH}/queries"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_lsqb.log"
touch "${LOG_FILE}"

MAIN_CLASS="feldera.experiments.Main"
INPUT_BASE="/home/ubuntu/data"

queries=("lsqb6")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera.sh" >> "${LOG_FILE}" 2>&1

  echo "Running ${query}..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" \
    "${MAIN_CLASS}" -c "${QUERIES_PATH}/lsqb/${query}.sql" -i "${INPUT_BASE}/lsqb/sf30" -p >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: ${query} did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"

  bash "${CURRENT_PATH}/stop_feldera.sh" >> "${LOG_FILE}" 2>&1
done
