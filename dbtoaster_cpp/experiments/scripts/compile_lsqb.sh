#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/hpps"
cd "${BACKEND_PATH}"

echo "Compiling lsqb6..."
rm -f "${EXPERIMENTS_PATH}/hpps/lsqb6.hpp"
sbt "toast -l cpp -O3 -o ${EXPERIMENTS_PATH}/hpps/lsqb6.hpp ${EXPERIMENTS_PATH}/queries/lsqb/lsqb6.sql"

echo "Done."
