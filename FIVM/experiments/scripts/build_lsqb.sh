#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/bin"

echo "Building lsqb6..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/lsqb6.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/lsqb/application_lsqb.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/lsqb6"

echo "Done."
