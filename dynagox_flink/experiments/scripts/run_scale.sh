#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
LOG_PATH="${EXPERIMENTS_PATH}/log"
mkdir -p "${LOG_PATH}"
LOG_FILE="${LOG_PATH}/run_scale.log"
touch "${LOG_FILE}"

MAIN_CLASS="dynagox.experiments.Main"

scales=("x1" "x2" "x5" "x20")
for scale in "${scales[@]}"; do
  echo "Running job19[${scale}]..." >> "${LOG_FILE}"
  timeout -s SIGKILL "${TIMEOUT_DURATION}" "${JAVA11_HOME}/bin/java" \
    -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "job19" \
    -i "${EXPERIMENT_DATA}/job/${scale}/job19_all.csv" >> "${LOG_FILE}" 2>&1
  if [ $? -eq 137 ]; then
    echo "Timeout: job19[${scale}] did not complete within ${TIMEOUT_DURATION}." >> "${LOG_FILE}"
  fi
  echo "" >> "${LOG_FILE}"
done
