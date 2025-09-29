#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

cd "${FRONTEND_PATH}"

echo "Compiling DBToaster frontend..."
make
echo "Done."
