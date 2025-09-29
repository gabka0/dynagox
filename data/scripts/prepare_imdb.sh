#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/imdb"

echo "Downloading DuckDB..."
rm -f "${DATA_PATH}/imdb/duckdb_cli-linux-amd64.zip"
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb/releases/download/v1.3.1/duckdb_cli-linux-amd64.zip
unzip -d "${DATA_PATH}/imdb/" "${DATA_PATH}/imdb/duckdb_cli-linux-amd64.zip"
rm -f "${DATA_PATH}/imdb/duckdb_cli-linux-amd64.zip"

cd "${DATA_PATH}/imdb"
rm -f "${DATA_PATH}/imdb/convert.sql"
touch "${DATA_PATH}/imdb/convert.sql"

echo "Downloading imdb data..."
rm -f "${DATA_PATH}/imdb/*.csv"

wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_aka_name.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_cast_info.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_char_name.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_company_name.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_company_type.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_info_type.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_keyword.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_kind_type.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_movie_companies.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_movie_info.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_movie_info_idx.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_movie_keyword.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_name.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_person_info.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_role_type.parquet
wget -P "${DATA_PATH}/imdb/" https://github.com/duckdb/duckdb-data/releases/download/v1.0/job_title.parquet

echo "CREATE TABLE aka_name AS SELECT * FROM 'job_aka_name.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY aka_name TO 'aka_name.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE cast_info AS SELECT * FROM 'job_cast_info.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY (SELECT id, person_id, movie_id, CAST(person_role_id AS BIGINT) AS person_role_id, role_id from cast_info) TO 'cast_info.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE char_name AS SELECT * FROM 'job_char_name.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY char_name TO 'char_name.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE company_name AS SELECT * FROM 'job_company_name.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY company_name TO 'company_name.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE company_type AS SELECT * FROM 'job_company_type.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY company_type TO 'company_type.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE info_type AS SELECT * FROM 'job_info_type.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY info_type TO 'info_type.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE keyword AS SELECT * FROM 'job_keyword.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY keyword TO 'keyword.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE kind_type AS SELECT * FROM 'job_kind_type.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY kind_type TO 'kind_type.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE movie_companies AS SELECT * FROM 'job_movie_companies.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY movie_companies TO 'movie_companies.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE movie_info AS SELECT * FROM 'job_movie_info.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY movie_info TO 'movie_info.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE movie_info_idx AS SELECT * FROM 'job_movie_info_idx.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY movie_info_idx TO 'movie_info_idx.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE movie_keyword AS SELECT * FROM 'job_movie_keyword.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY movie_keyword TO 'movie_keyword.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE name AS SELECT * FROM 'job_name.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY name TO 'name.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE person_info AS SELECT * FROM 'job_person_info.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY person_info TO 'person_info.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE role_type AS SELECT * FROM 'job_role_type.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY role_type TO 'role_type.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

echo "CREATE TABLE title AS SELECT * FROM 'job_title.parquet';" >> "${DATA_PATH}/imdb/convert.sql"
echo "COPY (SELECT id, kind_id from title) TO 'title.csv' (HEADER, DELIMITER ',');" >> "${DATA_PATH}/imdb/convert.sql"

"${DATA_PATH}/imdb/duckdb" < "${DATA_PATH}/imdb/convert.sql"

rm -f "${DATA_PATH}/imdb/convert.sql"
rm -f "${DATA_PATH}/imdb/duckdb"
rm -f "${DATA_PATH}/job/*.parquet"

echo "Done."
