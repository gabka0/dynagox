#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

cd "${BACKEND_PATH}"

echo "Compiling DBToaster backend..."

mkdir -p "${BACKEND_PATH}/ddbtoaster/conf/"
rm -f "${BACKEND_PATH}/ddbtoaster/conf/ddbt.properties"
touch "${BACKEND_PATH}/ddbtoaster/conf/ddbt.properties"
echo "ddbt.base_repo=${BASE_PATH}/dbtoaster-frontend" >> "${BACKEND_PATH}/ddbtoaster/conf/ddbt.properties"
echo "ddbt.data_repo=${BASE_PATH}/dbtoaster-experiments-data" >> "${BACKEND_PATH}/ddbtoaster/conf/ddbt.properties"

sbt clean
sbt compile
sbt release
echo "Done."
