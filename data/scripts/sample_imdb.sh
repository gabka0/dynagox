#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/imdb_sample"
rm -f "${DATA_PATH}/imdb_sample/*"

k=50

copy_files=("char_name.csv" "company_name.csv" "company_type.csv" "info_type.csv"
  "keyword.csv" "kind_type.csv" "name.csv" "role_type.csv" "title.csv")
for copy_file in "${copy_files[@]}"; do
  cp "${DATA_PATH}/imdb/${copy_file}" "${DATA_PATH}/imdb_sample/${copy_file}"
done

sample_files=("aka_name.csv" "cast_info.csv" "movie_companies.csv" "movie_info.csv"
  "movie_info_idx.csv" "movie_keyword.csv" "person_info.csv")
for sample_file in "${sample_files[@]}"; do
  total_lines=$(wc -l < "${DATA_PATH}/imdb/${sample_file}")
  n=$((total_lines / k))

  echo "Sampling ${n} lines from ${sample_file}..."
  head -n1 "${DATA_PATH}/imdb/${sample_file}" > "${DATA_PATH}/imdb_sample/${sample_file}"
  tail -n +2 "${DATA_PATH}/imdb/${sample_file}" | shuf -n "${n}" >> "${DATA_PATH}/imdb_sample/${sample_file}"
done
