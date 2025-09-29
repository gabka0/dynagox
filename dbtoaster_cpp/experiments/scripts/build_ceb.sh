#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

RELEASE_PATH="${BACKEND_PATH}/ddbtoaster/release"

mkdir -p "${EXPERIMENTS_PATH}/bin"
cd "${BACKEND_PATH}"

queries=("ceb1" "ceb2" "ceb3" "ceb4")
for query in "${queries[@]}"; do
  echo "Building ${query}..."
  g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
    -I "${RELEASE_PATH}/lib/dbt_c++/lib/" -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
    -include "${EXPERIMENTS_PATH}/hpps/${query}.hpp" \
    -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/ceb/application_${query}.h" \
    -o "${EXPERIMENTS_PATH}/bin/${query}"
done

echo "Done."
