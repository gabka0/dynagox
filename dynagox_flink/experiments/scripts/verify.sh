#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
VERIFY_PATH="${EXPERIMENTS_PATH}/verify"
mkdir -p "${VERIFY_PATH}"

MAIN_CLASS="dynagox.experiments.Main"

queries=("line5c" "line5s" "line5m" "line5cd" "dumbbell")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/graph/insert/graph.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("line5gc")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/graph/insert/graph.csv" -m "full" -p >> "${OUT_FILE}" 2>&1

  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_delta.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/graph/insert/graph.csv" -m "delta" -p >> "${OUT_FILE}" 2>&1
done

queries=("lsqb6")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/lsqb/sf1/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("job8" "job16" "job19")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/job/x2/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/ceb/x2/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("tpch3" "tpch5" "tpch8" "tpch9" "tpch10")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/tpch/insert/${query}_all.csv" -m "full" -p >> "${OUT_FILE}" 2>&1

  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_delta.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/tpch/insert/${query}_all.csv" -m "delta" -p >> "${OUT_FILE}" 2>&1
done

queries=("tpcds15" "tpcds19")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_full.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/tpcds/insert/${query}_all.csv" -m "full" -p >> "${OUT_FILE}" 2>&1

  OUT_FILE="${VERIFY_PATH}/${query}_dynagox_delta.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${JAVA11_HOME}/bin/java" -cp "${TARGET_PATH}/dynagox-1.0-SNAPSHOT.jar" "${MAIN_CLASS}" -q "${query}" \
    -i "${EXPERIMENT_DATA_TINY}/tpcds/insert/${query}_all.csv" -m "delta" -p >> "${OUT_FILE}" 2>&1
done
