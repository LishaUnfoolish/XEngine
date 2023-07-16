#!/usr/bin/env bash

BOLD='\033[1m'
RED='\033[0;31m'
GREEN='\033[32m'
WHITE='\033[34m'
YELLOW='\033[33m'
NO_COLOR='\033[0m'

function info() {
    (>&2 echo -e "[${WHITE}${BOLD}INFO${NO_COLOR}] $*")
}

function error() {
    (>&2 echo -e "[${RED}ERROR${NO_COLOR}] $*")
}

function warning() {
    (>&2 echo -e "${YELLOW}[WARNING] $*${NO_COLOR}")
}

function ok() {
    (>&2 echo -e "[${GREEN}${BOLD} OK ${NO_COLOR}] $*")
}

export PROJECT_NAME="xengine"
export DOCKER_IMAGE="lixiaoxmm/xengine:v1.6"
export USER_ID=$(id -u)
export GRP=$(id -g -n)
export GRP_ID=$(id -g)
export DOCKER_IMAGE=${DOCKER_IMAGE}
export ROOT_DIR=${ROOT_DIR}
export USER=${USER}
export PROJECT_NAME=${PROJECT_NAME}
export RUN_DEV=${PROJECT_NAME}_$USER
echo "${PROJECT_NAME}"
