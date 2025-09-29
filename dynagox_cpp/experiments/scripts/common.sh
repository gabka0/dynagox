#!/bin/bash

COMMON_SCRIPT=$(readlink -f $0)
SCRIPTS_PATH=$(dirname "${COMMON_SCRIPT}")
EXPERIMENTS_PATH=$(dirname "${SCRIPTS_PATH}")
BASE_PATH=$(dirname "${EXPERIMENTS_PATH}")
ROOT_PATH=$(dirname "${BASE_PATH}")
IMPL_PATH="${BASE_PATH}/implement"

source "${ROOT_PATH}/common.sh"
