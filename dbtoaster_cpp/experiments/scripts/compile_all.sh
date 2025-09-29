#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

echo "Compiling all..."

bash "${CURRENT_PATH}/compile_frontend.sh"
bash "${CURRENT_PATH}/compile_backend.sh"

bash "${CURRENT_PATH}/compile_graph.sh"
bash "${CURRENT_PATH}/compile_lsqb.sh"
bash "${CURRENT_PATH}/compile_job.sh"
bash "${CURRENT_PATH}/compile_ceb.sh"
bash "${CURRENT_PATH}/compile_tpch.sh"
bash "${CURRENT_PATH}/compile_tpcds.sh"

echo "Done."
