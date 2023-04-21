#!/usr/bin/env bash
export PROJECT_NAME="xengine"
export DOCKER_IMAGE="lixiaoxmm/xengine:v1.4"
export USER_ID=$(id -u)
export GRP=$(id -g -n)
export GRP_ID=$(id -g)
export DOCKER_IMAGE=${DOCKER_IMAGE}
export ROOT_DIR=${ROOT_DIR}
export USER=${USER}
export PROJECT_NAME=${PROJECT_NAME}
export RUN_DEV=${PROJECT_NAME}_$USER
echo "${PROJECT_NAME}"
