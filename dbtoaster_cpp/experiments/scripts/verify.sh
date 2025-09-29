#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BIN_PATH="${EXPERIMENTS_PATH}/bin"
VERIFY_PATH="${EXPERIMENTS_PATH}/verify"
mkdir -p "${VERIFY_PATH}"

queries=("line5c" "line5s" "line5cd" "line5gc" "dumbbell")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dbtoaster.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${BIN_PATH}/${query}" -i "${EXPERIMENT_DATA_TINY}/graph/insert/graph.csv" >> "${OUT_FILE}" 2>&1
done

queries=("lsqb6")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dbtoaster.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${BIN_PATH}/${query}" -i "${EXPERIMENT_DATA_TINY}/lsqb/sf1/${query}_all.csv" >> "${OUT_FILE}" 2>&1
done

queries=("job8" "job16" "job19")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dbtoaster.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${BIN_PATH}/${query}" -i "${EXPERIMENT_DATA_TINY}/job/x2/${query}_all.csv" >> "${OUT_FILE}" 2>&1
done

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dbtoaster.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${BIN_PATH}/${query}" -i "${EXPERIMENT_DATA_TINY}/ceb/x2/${query}_all.csv" >> "${OUT_FILE}" 2>&1
done

queries=("tpch3" "tpch5" "tpch8" "tpch9" "tpch10")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dbtoaster.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${BIN_PATH}/${query}" -i "${EXPERIMENT_DATA_TINY}/tpch/insert/${query}_all.csv" >> "${OUT_FILE}" 2>&1
done

queries=("tpcds15" "tpcds19")
for query in "${queries[@]}"; do
  OUT_FILE="${VERIFY_PATH}/${query}_dbtoaster.out"
  rm -f "${OUT_FILE}"
  touch "${OUT_FILE}"
  "${BIN_PATH}/${query}" -i "${EXPERIMENT_DATA_TINY}/tpcds/insert/${query}_all.csv" >> "${OUT_FILE}" 2>&1
done
