#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

BUILD_PATH="${IMPL_PATH}/build"
rm -rf "${BUILD_PATH}"
mkdir -p "${BUILD_PATH}"

echo "Compiling all..."

cd "${BUILD_PATH}"
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

echo "Done."
