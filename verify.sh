#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

bash "${CURRENT_PATH}/FIVM/experiments/scripts/verify.sh"
bash "${CURRENT_PATH}/dbtoaster_cpp/experiments/scripts/verify.sh"
bash "${CURRENT_PATH}/dynagox_cpp/experiments/scripts/verify.sh"
bash "${CURRENT_PATH}/dynagox_flink/experiments/scripts/verify.sh"
bash "${CURRENT_PATH}/crown_cpp/experiments/scripts/verify.sh"
bash "${CURRENT_PATH}/feldera/experiments/scripts/verify.sh"
bash "${CURRENT_PATH}/duckdb/experiments/scripts/verify.sh"

cd "${CURRENT_PATH}/ResultVerifier"
sbt clean
sbt assembly

FIVM_VERIFY_PATH="${CURRENT_PATH}/FIVM/experiments/verify"
DBTOASTER_VERIFY_PATH="${CURRENT_PATH}/dbtoaster_cpp/experiments/verify"
DYNAGOX_CPP_VERIFY_PATH="${CURRENT_PATH}/dynagox_cpp/experiments/verify"
DYNAGOX_FLINK_VERIFY_PATH="${CURRENT_PATH}/dynagox_flink/experiments/verify"
CROWN_VERIFY_PATH="${CURRENT_PATH}/crown_cpp/experiments/verify"
FELDERA_VERIFY_PATH="${CURRENT_PATH}/feldera/experiments/verify"
DUCKDB_VERIFY_PATH="${CURRENT_PATH}/duckdb/experiments/verify"

JAR_FILE="${CURRENT_PATH}/ResultVerifier/target/scala-2.12/ResultVerifier.jar"
MAIN_CLASS="dynagox.experiments.Main"

queries=("line5c" "line5s")
for query in "${queries[@]}"; do
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" \
    -a "${FIVM_VERIFY_PATH}/${query}_fivm.out" \
    -b "${DBTOASTER_VERIFY_PATH}/${query}_dbtoaster.out" \
    -c "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_full.out" \
    -e "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_full.out" \
    -g "${CROWN_VERIFY_PATH}/${query}_crown_full.out" \
    -i "${FELDERA_VERIFY_PATH}/${query}_feldera.out" \
    -j "${DUCKDB_VERIFY_PATH}/${query}_duckdb.out"
done

java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "line5m" \
  -a "${FIVM_VERIFY_PATH}/line5m_fivm.out" \
  -c "${DYNAGOX_CPP_VERIFY_PATH}/line5m_dynagox_full.out" \
  -e "${DYNAGOX_FLINK_VERIFY_PATH}/line5m_dynagox_full.out" \
  -g "${CROWN_VERIFY_PATH}/line5m_crown_full.out" \
  -i "${FELDERA_VERIFY_PATH}/line5m_feldera.out" \
  -j "${DUCKDB_VERIFY_PATH}/line5m_duckdb.out"

java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "line5cd" \
  -b "${DBTOASTER_VERIFY_PATH}/line5cd_dbtoaster.out" \
  -c "${DYNAGOX_CPP_VERIFY_PATH}/line5cd_dynagox_full.out" \
  -e "${DYNAGOX_FLINK_VERIFY_PATH}/line5cd_dynagox_full.out" \
  -g "${CROWN_VERIFY_PATH}/line5cd_crown_full.out" \
  -i "${FELDERA_VERIFY_PATH}/line5cd_feldera.out" \
  -j "${DUCKDB_VERIFY_PATH}/line5cd_duckdb.out"

java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "line5gc" \
  -a "${FIVM_VERIFY_PATH}/line5gc_fivm.out" \
  -b "${DBTOASTER_VERIFY_PATH}/line5gc_dbtoaster.out" \
  -c "${DYNAGOX_CPP_VERIFY_PATH}/line5gc_dynagox_full.out" \
  -d "${DYNAGOX_CPP_VERIFY_PATH}/line5gc_dynagox_delta.out" \
  -e "${DYNAGOX_FLINK_VERIFY_PATH}/line5gc_dynagox_full.out" \
  -f "${DYNAGOX_FLINK_VERIFY_PATH}/line5gc_dynagox_delta.out" \
  -g "${CROWN_VERIFY_PATH}/line5gc_crown_full.out" \
  -h "${CROWN_VERIFY_PATH}/line5gc_crown_delta.out" \
  -i "${FELDERA_VERIFY_PATH}/line5gc_feldera.out" \
  -j "${DUCKDB_VERIFY_PATH}/line5gc_duckdb.out"

java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "dumbbell" \
  -a "${FIVM_VERIFY_PATH}/dumbbell_fivm.out" \
  -b "${DBTOASTER_VERIFY_PATH}/dumbbell_dbtoaster.out" \
  -c "${DYNAGOX_CPP_VERIFY_PATH}/dumbbell_dynagox_full.out" \
  -e "${DYNAGOX_FLINK_VERIFY_PATH}/dumbbell_dynagox_full.out" \
  -g "${CROWN_VERIFY_PATH}/dumbbell_crown_full.out" \
  -i "${FELDERA_VERIFY_PATH}/dumbbell_feldera.out" \
  -j "${DUCKDB_VERIFY_PATH}/dumbbell_duckdb.out"

queries=("lsqb6" "job8" "job16" "job19" "ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" \
    -a "${FIVM_VERIFY_PATH}/${query}_fivm.out" \
    -b "${DBTOASTER_VERIFY_PATH}/${query}_dbtoaster.out" \
    -c "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_full.out" \
    -e "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_full.out" \
    -g "${CROWN_VERIFY_PATH}/${query}_crown_full.out" \
    -i "${FELDERA_VERIFY_PATH}/${query}_feldera.out" \
    -j "${DUCKDB_VERIFY_PATH}/${query}_duckdb.out"
done

queries=("tpch3" "tpch5")
for query in "${queries[@]}"; do
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" \
    -a "${FIVM_VERIFY_PATH}/${query}_fivm.out" \
    -b "${DBTOASTER_VERIFY_PATH}/${query}_dbtoaster.out" \
    -c "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_full.out" \
    -d "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_delta.out" \
    -e "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_full.out" \
    -f "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_delta.out" \
    -g "${CROWN_VERIFY_PATH}/${query}_crown_full.out" \
    -h "${CROWN_VERIFY_PATH}/${query}_crown_delta.out" \
    -i "${FELDERA_VERIFY_PATH}/${query}_feldera.out" \
    -j "${DUCKDB_VERIFY_PATH}/${query}_duckdb.out"
done

queries=("tpch8" "tpch9")
for query in "${queries[@]}"; do
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" \
    -b "${DBTOASTER_VERIFY_PATH}/${query}_dbtoaster.out" \
    -c "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_full.out" \
    -d "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_delta.out" \
    -e "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_full.out" \
    -f "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_delta.out" \
    -g "${CROWN_VERIFY_PATH}/${query}_crown_full.out" \
    -h "${CROWN_VERIFY_PATH}/${query}_crown_delta.out" \
    -i "${FELDERA_VERIFY_PATH}/${query}_feldera.out" \
    -j "${DUCKDB_VERIFY_PATH}/${query}_duckdb.out"
done

java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "tpch10" \
  -a "${FIVM_VERIFY_PATH}/tpch10_fivm.out" \
  -b "${DBTOASTER_VERIFY_PATH}/tpch10_dbtoaster.out" \
  -c "${DYNAGOX_CPP_VERIFY_PATH}/tpch10_dynagox_full.out" \
  -d "${DYNAGOX_CPP_VERIFY_PATH}/tpch10_dynagox_delta.out" \
  -e "${DYNAGOX_FLINK_VERIFY_PATH}/tpch10_dynagox_full.out" \
  -f "${DYNAGOX_FLINK_VERIFY_PATH}/tpch10_dynagox_delta.out" \
  -g "${CROWN_VERIFY_PATH}/tpch10_crown_full.out" \
  -h "${CROWN_VERIFY_PATH}/tpch10_crown_delta.out" \
  -i "${FELDERA_VERIFY_PATH}/tpch10_feldera.out" \
  -j "${DUCKDB_VERIFY_PATH}/tpch10_duckdb.out"

queries=("tpcds15" "tpcds19")
for query in "${queries[@]}"; do
  java -cp "${JAR_FILE}" "${MAIN_CLASS}" -q "${query}" \
    -a "${FIVM_VERIFY_PATH}/${query}_fivm.out" \
    -b "${DBTOASTER_VERIFY_PATH}/${query}_dbtoaster.out" \
    -c "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_full.out" \
    -d "${DYNAGOX_CPP_VERIFY_PATH}/${query}_dynagox_delta.out" \
    -e "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_full.out" \
    -f "${DYNAGOX_FLINK_VERIFY_PATH}/${query}_dynagox_delta.out" \
    -g "${CROWN_VERIFY_PATH}/${query}_crown_full.out" \
    -h "${CROWN_VERIFY_PATH}/${query}_crown_delta.out" \
    -i "${FELDERA_VERIFY_PATH}/${query}_feldera.out" \
    -j "${DUCKDB_VERIFY_PATH}/${query}_duckdb.out"
done

echo "Done."
