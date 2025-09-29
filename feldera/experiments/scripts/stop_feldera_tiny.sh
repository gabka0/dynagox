#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

docker compose -f "${DEPLOY_PATH}/docker-compose-tiny.yml" stop pipeline-manager > /dev/null 2>&1
sleep 5s
docker compose -f "${DEPLOY_PATH}/docker-compose-tiny.yml" down -v > /dev/null 2>&1
sleep 5s
