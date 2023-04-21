#!/usr/bin/env bash
BOLD='\033[1m'
RED='\033[0;31m'
GREEN='\033[32m'
WHITE='\033[34m'
YELLOW='\033[33m'
NONE='\033[0m'

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../" && pwd -P )"
source ${ROOT_DIR}/docker/scripts/docker_common.sh


function info() {
  echo -e "[${WHITE}${BOLD}INFO${NONE}] $*"
}
function error() {
  echo >&2 -e "[${RED}ERROR${NONE}] $*"
}
function ok() {
  echo -e "[${GREEN}${BOLD} OK ${NONE}] $*"
}

function print_usage() {
  echo -e "\n${RED}Usage${NONE}:
  .${BOLD}/docker_manager.sh${NONE} [OPTION]"

  echo -e "\n${RED}Options${NONE}:
  ${BLUE}build_image${NONE}: Build containers.
  ${BLUE}push_image${NONE}: Build and push containers to hub.
  "
}


function build_image() {
  set -e
  docker build -f ${ROOT_DIR}/docker/Dockerfile -t ${PROJECT_NAME}:test .
  if [ $? -ne 0 ]; then
    error "Failed to build base docker image"
    exit 1
  fi
}

function push_image() {
  set -e
  build_image
  if [ $? -ne 0 ]; then
    error "Failed to push docker image"
    exit 1
  fi
  docker tag ${PROJECT_NAME}:test ${DOCKER_IMAGE}
  if [ $? -ne 0 ]; then
    error "Failed to docker tag."
    exit 2
  fi
  docker push "${DOCKER_IMAGE}"
  if [ $? -ne 0 ]; then
    error "Failed to push docker image"
    exit 3
  fi
  ok "Successfully built and pushed to ${DOCKER_IMAGE}"
}


function main() {
  local cmd=$1
  shift
  case $cmd in
  build_image)
    build_image "$@"
    ;;
  push_image)
    push_image "$@"
    ;;
  *)
    print_usage
    ;;
  esac
}
main "$@"
