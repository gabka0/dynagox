#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/m3s"
mkdir -p "${EXPERIMENTS_PATH}/hpps"

echo "Compiling lsqb6..."
rm -f "${EXPERIMENTS_PATH}/m3s/lsqb6.m3"
rm -f "${EXPERIMENTS_PATH}/hpps/lsqb6.hpp"
"${BIN_PATH}/run_frontend.sh" --single "${EXPERIMENTS_PATH}/queries/lsqb/lsqb6.sql" -o "${EXPERIMENTS_PATH}/m3s/lsqb6.m3"
"${BIN_PATH}/run_backend.sh" "${EXPERIMENTS_PATH}/m3s/lsqb6.m3" -o "${EXPERIMENTS_PATH}/hpps/lsqb6.hpp"

echo "Done."
