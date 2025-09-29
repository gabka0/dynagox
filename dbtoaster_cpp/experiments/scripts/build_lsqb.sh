#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

RELEASE_PATH="${BACKEND_PATH}/ddbtoaster/release"

mkdir -p "${EXPERIMENTS_PATH}/bin"
cd "${BACKEND_PATH}"

echo "Building lsqb6..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/lsqb6.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/lsqb/application_lsqb.h" \
  -o "${EXPERIMENTS_PATH}/bin/lsqb6"

echo "Done."
