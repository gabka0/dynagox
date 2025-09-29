#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/bin"

queries=("job8" "job16" "job19")
for query in "${queries[@]}"; do
  echo "Building ${query}..."
  g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
    -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
    -include "${EXPERIMENTS_PATH}/hpps/${query}.hpp" \
    -include "${EXPERIMENTS_PATH}/src/application/job/application_${query}.hpp" \
    -o "${EXPERIMENTS_PATH}/bin/${query}"
done

echo "Building job8s..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/job8s.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/job/application_job8.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/job8s"

queries=("job8m1" "job8m2")
for query in "${queries[@]}"; do
  echo "Building ${query}..."
  g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
    -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
    -I "${EXPERIMENTS_PATH}/src/ring" -include "${EXPERIMENTS_PATH}/hpps/${query}.hpp" \
    -include "${EXPERIMENTS_PATH}/src/application/job/application_job8.hpp" \
    -o "${EXPERIMENTS_PATH}/bin/${query}"
done

echo "Done."
