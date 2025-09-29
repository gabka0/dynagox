#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${DATA_PATH}/ldbc_tiny"

echo "Downloading spark-3.2.2-bin-hadoop3.2..."
rm -rf "${DATA_PATH}/ldbc_tiny/spark-3.2.2-bin-hadoop3.2.tgz"
rm -rf "${DATA_PATH}/ldbc_tiny/spark-3.2.2-bin-hadoop3.2"
cd "${DATA_PATH}/ldbc_tiny"
wget https://archive.apache.org/dist/spark/spark-3.2.2/spark-3.2.2-bin-hadoop3.2.tgz
tar -zxvf spark-3.2.2-bin-hadoop3.2.tgz
rm -rf "${DATA_PATH}/ldbc_tiny/spark-3.2.2-bin-hadoop3.2.tgz"
SPARK_PATH="${DATA_PATH}/ldbc_tiny/spark-3.2.2-bin-hadoop3.2"

echo "Downloading ldbc_snb_datagen_spark..."
rm -rf "${DATA_PATH}/ldbc_tiny/ldbc_snb_datagen_spark"
cd "${DATA_PATH}/ldbc_tiny"
git clone git@github.com:ldbc/ldbc_snb_datagen_spark.git
DATAGEN_PATH="${DATA_PATH}/ldbc_tiny/ldbc_snb_datagen_spark"

echo "Building ldbc_snb_datagen_spark..."
cd "${DATAGEN_PATH}"
sbt assembly

SCALE_FACTOR="1"
echo "Running ldbc_snb_datagen_spark[SF=${SCALE_FACTOR}]..."
export PLATFORM_VERSION=$(sbt -batch -error 'print platformVersion')
export DATAGEN_VERSION=$(sbt -batch -error 'print version')
export LDBC_SNB_DATAGEN_JAR=$(sbt -batch -error 'print assembly / assemblyOutputPath')
export SPARK_HOME="${SPARK_PATH}"
export PATH="${SPARK_HOME}/bin:${PATH}"

"${DATAGEN_PATH}/tools/run.py" --parallelism 1 --memory 256g -- --format csv --scale-factor "${SCALE_FACTOR}" --mode raw

DYNAMIC_PATH="${DATAGEN_PATH}/out/graphs/csv/raw/composite-merged-fk/dynamic/"
rm -rf "${DATA_PATH}/ldbc_tiny/Person_knows_Person/"
rm -rf "${DATA_PATH}/ldbc_tiny/Person_hasInterest_Tag/"
mv "${DYNAMIC_PATH}/Person_knows_Person/" "${DATA_PATH}/ldbc_tiny/"
mv "${DYNAMIC_PATH}/Person_hasInterest_Tag/" "${DATA_PATH}/ldbc_tiny/"

rm -rf "${SPARK_PATH}"
rm -rf "${DATAGEN_PATH}"

echo "Done."
