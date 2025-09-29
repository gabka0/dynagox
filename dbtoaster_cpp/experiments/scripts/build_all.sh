#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

echo "Building all..."

bash "${CURRENT_PATH}/build_graph.sh"
bash "${CURRENT_PATH}/build_lsqb.sh"
bash "${CURRENT_PATH}/build_job.sh"
bash "${CURRENT_PATH}/build_ceb.sh"
bash "${CURRENT_PATH}/build_tpch.sh"
bash "${CURRENT_PATH}/build_tpcds.sh"

echo "Done."
