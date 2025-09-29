#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
VERIFY_PATH="${EXPERIMENTS_PATH}/verify"
mkdir -p "${VERIFY_PATH}"

MAIN_CLASS="duckdb.experiments.Main"
JAR_FILE="${TARGET_PATH}/duckdb-1.0-SNAPSHOT-jar-with-dependencies.jar"

queries=("line5c" "line5s" "line5m" "line5cd" "line5gc" "dumbbell")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_duckdb.out"
  rm -f "${OUT_FILE}"
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" -i "${EXPERIMENT_DATA_TINY}/graph/insert/graph.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("lsqb6")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_duckdb.out"
  rm -f "${OUT_FILE}"
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" -i "${EXPERIMENT_DATA_TINY}/lsqb/sf1/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("job8" "job16" "job19")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_duckdb.out"
  rm -f "${OUT_FILE}"
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" -i "${EXPERIMENT_DATA_TINY}/job/x2/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_duckdb.out"
  rm -f "${OUT_FILE}"
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" -i "${EXPERIMENT_DATA_TINY}/ceb/x2/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("tpch3" "tpch5" "tpch8" "tpch9" "tpch10")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_duckdb.out"
  rm -f "${OUT_FILE}"
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" -i "${EXPERIMENT_DATA_TINY}/tpch/insert/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done

queries=("tpcds15" "tpcds19")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_duckdb.out"
  rm -f "${OUT_FILE}"
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" -i "${EXPERIMENT_DATA_TINY}/tpcds/insert/${query}_all.csv" -p >> "${OUT_FILE}" 2>&1
done
