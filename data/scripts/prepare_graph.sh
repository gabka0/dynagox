#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/graph"

echo "Downloading epinions..."
rm -f "${DATA_PATH}/graph/epinions.txt"
rm -f "${DATA_PATH}/graph/soc-Epinions1.txt"
rm -f "${DATA_PATH}/graph/soc-Epinions1.txt.gz"

wget -P "${DATA_PATH}/graph/" https://snap.stanford.edu/data/soc-Epinions1.txt.gz
gunzip -c "${DATA_PATH}/graph/soc-Epinions1.txt.gz" > "${DATA_PATH}/graph/soc-Epinions1.txt"
rm -f "${DATA_PATH}/graph/soc-Epinions1.txt.gz"

tail -n +5 "${DATA_PATH}/graph/soc-Epinions1.txt" > "${DATA_PATH}/graph/epinions.csv"
rm -f "${DATA_PATH}/graph/soc-Epinions1.txt"
sed -i "s/\t/|/g" "${DATA_PATH}/graph/epinions.csv"

echo "Done."
