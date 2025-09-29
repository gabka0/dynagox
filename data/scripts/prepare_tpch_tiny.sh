#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/tpch_tiny"

echo "Downloading DuckDB..."
rm -f "${DATA_PATH}/tpch_tiny/duckdb_cli-linux-amd64.zip"
wget -P "${DATA_PATH}/tpch_tiny/" https://github.com/duckdb/duckdb/releases/download/v1.1.3/duckdb_cli-linux-amd64.zip
unzip -d "${DATA_PATH}/tpch_tiny/" "${DATA_PATH}/tpch_tiny/duckdb_cli-linux-amd64.zip"
rm -f "${DATA_PATH}/tpch_tiny/duckdb_cli-linux-amd64.zip"

cd "${DATA_PATH}/tpch_tiny"
rm -f "${DATA_PATH}/tpch_tiny/export.sql"
touch "${DATA_PATH}/tpch_tiny/export.sql"

SCALE_FACTOR="0.05"
echo "Generating tpch data[SF=${SCALE_FACTOR}]..."
rm -f "${DATA_PATH}/tpch_tiny/*.csv"

echo "INSTALL tpch;" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "LOAD tpch;" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "CALL dbgen(sf = ${SCALE_FACTOR});" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY customer TO 'customer.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY lineitem TO 'lineitem.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY nation TO 'nation.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY orders TO 'orders.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY part TO 'part.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY partsupp TO 'partsupp.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY region TO 'region.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"
echo "COPY supplier TO 'supplier.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpch_tiny/export.sql"

"${DATA_PATH}/tpch_tiny/duckdb" < "${DATA_PATH}/tpch_tiny/export.sql"

rm -f "${DATA_PATH}/tpch_tiny/export.sql"
rm -f "${DATA_PATH}/tpch_tiny/duckdb"

echo "Done."
