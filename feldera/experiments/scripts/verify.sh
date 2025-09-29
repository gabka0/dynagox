#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

TARGET_PATH="${BASE_PATH}/target"
QUERIES_PATH="${EXPERIMENTS_PATH}/queries"
VERIFY_PATH="${EXPERIMENTS_PATH}/verify"
mkdir -p "${VERIFY_PATH}"

MAIN_CLASS="feldera.experiments.Main"
INPUT_BASE="/home/ubuntu/data"

queries=("line5c" "line5s" "line5m" "line5cd" "line5gc")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/line5/${query}.sql" -i "${INPUT_BASE}/graph/insert" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done

queries=("dumbbell")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/dumbbell/${query}.sql" -i "${INPUT_BASE}/graph/insert" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done

queries=("lsqb6")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/lsqb/${query}.sql" -i "${INPUT_BASE}/lsqb/sf1" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done

queries=("job8" "job16" "job19")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/job/${query}.sql" -i "${INPUT_BASE}/job/x2" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/ceb/${query}.sql" -i "${INPUT_BASE}/ceb/x2" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done

queries=("tpch3" "tpch5" "tpch8" "tpch9" "tpch10")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/tpch/${query}.sql" -i "${INPUT_BASE}/tpch/insert" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done

queries=("tpcds15" "tpcds19")
for query in "${queries[@]}"; do
  bash "${CURRENT_PATH}/start_feldera_tiny.sh"
  OUT_FILE="${VERIFY_PATH}/${query}_feldera.out"
  rm -f "${OUT_FILE}"

  java -cp "${TARGET_PATH}/feldera-1.0-SNAPSHOT-jar-with-dependencies.jar" "${MAIN_CLASS}" \
    -c "${QUERIES_PATH}/tpcds/${query}.sql" -i "${INPUT_BASE}/tpcds/insert" 2>&1

  CONTAINER_ID=$(docker ps --format "{{.ID}}" -f "name=feldera-pipeline-manager-1")
  docker cp "${CONTAINER_ID}:/home/ubuntu/result.txt" "${OUT_FILE}"

  bash "${CURRENT_PATH}/stop_feldera_tiny.sh"
done
