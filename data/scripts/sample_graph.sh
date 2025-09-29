#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/graph_sample"
rm -f "${DATA_PATH}/graph_sample/*"

n=10000

echo "Sampling ${n} lines from epinions..."
shuf -n "${n}" "${DATA_PATH}/graph/epinions.csv" > "${DATA_PATH}/graph_sample/epinions.csv"
