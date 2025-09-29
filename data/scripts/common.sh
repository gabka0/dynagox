#!/bin/bash

COMMON_SCRIPT=$(readlink -f $0)
SCRIPTS_PATH=$(dirname "${COMMON_SCRIPT}")
DATA_PATH=$(dirname "${SCRIPTS_PATH}")
