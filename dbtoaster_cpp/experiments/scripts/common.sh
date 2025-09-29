#!/bin/bash

COMMON_SCRIPT=$(readlink -f $0)
SCRIPTS_PATH=$(dirname "${COMMON_SCRIPT}")
EXPERIMENTS_PATH=$(dirname "${SCRIPTS_PATH}")
BASE_PATH=$(dirname "${EXPERIMENTS_PATH}")
ROOT_PATH=$(dirname "${BASE_PATH}")
BACKEND_PATH="${BASE_PATH}/dbtoaster-backend"
FRONTEND_PATH="${BASE_PATH}/dbtoaster-frontend"

source "${ROOT_PATH}/common.sh"
