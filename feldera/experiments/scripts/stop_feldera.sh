#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

docker compose -f "${DEPLOY_PATH}/docker-compose.yml" stop pipeline-manager > /dev/null 2>&1
sleep 5s
docker compose -f "${DEPLOY_PATH}/docker-compose.yml" down -v > /dev/null 2>&1
sleep 5s

echo "feldera stopped."