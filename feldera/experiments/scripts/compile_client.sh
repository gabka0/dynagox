#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

cd "${BASE_PATH}"
echo "Compiling client..."
mvn clean package
echo "Done."
