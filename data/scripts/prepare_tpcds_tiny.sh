#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/tpcds_tiny"

echo "Downloading DuckDB..."
rm -f "${DATA_PATH}/tpcds_tiny/duckdb_cli-linux-amd64.zip"
wget -P "${DATA_PATH}/tpcds_tiny/" https://github.com/duckdb/duckdb/releases/download/v1.1.3/duckdb_cli-linux-amd64.zip
unzip -d "${DATA_PATH}/tpcds_tiny/" "${DATA_PATH}/tpcds_tiny/duckdb_cli-linux-amd64.zip"
rm -f "${DATA_PATH}/tpcds_tiny/duckdb_cli-linux-amd64.zip"

cd "${DATA_PATH}/tpcds_tiny"
rm -f "${DATA_PATH}/tpcds_tiny/export.sql"
touch "${DATA_PATH}/tpcds_tiny/export.sql"

SCALE_FACTOR="0.1"
echo "Generating tpcds data[SF=${SCALE_FACTOR}]..."
rm -f "${DATA_PATH}/tpcds_tiny/*.csv"

echo "INSTALL tpcds;" >> "${DATA_PATH}/tpcds_tiny/export.sql"
echo "LOAD tpcds;" >> "${DATA_PATH}/tpcds_tiny/export.sql"
echo "CALL dsdgen(sf = ${SCALE_FACTOR});" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT ss_sold_date_sk, ss_item_sk, ss_customer_sk, ss_store_sk, ss_ext_sales_price FROM store_sales
WHERE ss_sold_date_sk IS NOT NULL AND ss_item_sk IS NOT NULL AND ss_customer_sk IS NOT NULL
AND ss_store_sk IS NOT NULL AND ss_ext_sales_price IS NOT NULL)
TO 'store_sales.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT cs_sold_date_sk, cs_bill_customer_sk, cs_sales_price FROM catalog_sales
WHERE cs_sold_date_sk IS NOT NULL AND cs_bill_customer_sk IS NOT NULL AND cs_sales_price IS NOT NULL)
TO 'catalog_sales.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT s_store_sk FROM store WHERE s_store_sk IS NOT NULL)
TO 'store.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT c_customer_sk, c_current_addr_sk FROM customer WHERE c_customer_sk IS NOT NULL
AND c_current_addr_sk IS NOT NULL)
TO 'customer.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT ca_address_sk, ca_zip FROM customer_address WHERE ca_address_sk IS NOT NULL AND ca_zip IS NOT NULL)
TO 'customer_address.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT d_date_sk FROM date_dim WHERE d_date_sk IS NOT NULL)
TO 'date_dim.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

echo "COPY (SELECT i_item_sk, i_brand_id, i_brand, i_manufact_id, i_manufact FROM item WHERE i_item_sk IS NOT NULL
AND i_brand_id IS NOT NULL AND i_brand IS NOT NULL AND i_manufact_id IS NOT NULL AND i_manufact IS NOT NULL)
TO 'item.csv' (HEADER, DELIMITER '|');" >> "${DATA_PATH}/tpcds_tiny/export.sql"

"${DATA_PATH}/tpcds_tiny/duckdb" < "${DATA_PATH}/tpcds_tiny/export.sql"

rm -f "${DATA_PATH}/tpcds_tiny/export.sql"
rm -f "${DATA_PATH}/tpcds_tiny/duckdb"

echo "Done."
