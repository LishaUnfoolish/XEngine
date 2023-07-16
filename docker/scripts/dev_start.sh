#!/usr/bin/env bash
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@163.com
# @Time: 2022/11/20
# @Desc: 部署、启动和运行docker
# ***************************/
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../" && pwd -P)"
source ${ROOT_DIR}/docker/scripts/docker_common.sh
echo ${ROOT_DIR}
echo ${DOCKER_IMAGE}
function show_usage() {
  cat <<EOF
  Usage: $(basename $0) [options] ...
  OPTIONS:
      start                   star run containers.
      stop                   Stop all running containers.
EOF
  exit 0
}

function stop_containers() {
  running_containers=$(docker ps --format "{{.Names}}")

  for i in ${running_containers[*]}; do
    if [[ "$i" =~ ${PROJECT_NAME}_* ]]; then
      printf %-*s 70 "stopping container: $i ..."
      docker stop $i >/dev/null
      docker rm $i >/dev/null
      if [ $? -eq 0 ]; then
        printf "\033[32m[DONE]\033[0m\n"
      else
        printf "\033[31m[FAILED]\033[0m\n"
      fi
    fi
  done
}

function start_containers() {
  # start docker
  sudo service docker start
  # pull
  if docker image inspect "$DOCKER_IMAGE" &>/dev/null; then
    echo "The image $DOCKER_IMAGE exists."
  else
    docker pull $DOCKER_IMAGE
    if [ $? -ne 0 ]; then
      printf "Failed to pull docker image."
      exit 1
    fi
  fi

  docker ps -a --format "{{.Names}}" | grep "$RUN_DEV" 1>/dev/null
  if [ $? == 0 ]; then
    if [[ "$(docker inspect --format='{{.Config.Image}}' $RUN_DEV 2>/dev/null)" != "$DOCKER_IMAGE" ]]; then
      rm -rf $ROOT_DIR/build
    fi
    docker stop $RUN_DEV 1>/dev/null
    docker rm -v -f $RUN_DEV 1>/dev/null
  fi

  LOCAL_HOST=$(hostname)
  DOCKER_HOME="/home/$USER"
  if [ "$USER" == "root" ]; then
    DOCKER_HOME="/root"
  fi
  if [ ! -d "$HOME/.cache" ]; then
    mkdir "$HOME/.cache"
  fi

  printf "Starting docker container \"${RUN_DEV}\" ..."

  DOCKER_RUN="docker run"
  set -x
  ${DOCKER_RUN} -it \
    -d \
    --privileged \
    --name $RUN_DEV \
    -e DOCKER_USER=$USER \
    -e USER=$USER \
    -e DOCKER_USER_ID=$USER_ID \
    -e DOCKER_GRP="$GRP" \
    -e DOCKER_GRP_ID=$GRP_ID \
    -e DOCKER_IMG=$DOCKER_IMAGE \
    -v $ROOT_DIR:/${PROJECT_NAME} \
    --net host \
    -w /${PROJECT_NAME} \
    --add-host in_dev_docker:127.0.0.1 \
    --add-host ${LOCAL_HOST}:127.0.0.1 \
    --hostname in_dev_docker \
    --shm-size 2G \
    --pid=host \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    $DOCKER_IMAGE \
    /bin/bash
  if [ $? -ne 0 ]; then
    printf "Failed to start docker container \"${RUN_DEV}\" based on image: $DOCKER_IMAGE"
    exit 1
  fi
  set +x

  echo $RUN_DEV
  if [ "${USER}" != "root" ]; then
    eval docker exec $RUN_DEV bash -c "/${PROJECT_NAME}/docker/scripts/docker_adduser.sh"
  fi

  printf "Finished setting up ${PROJECT_NAME} docker environment. Now you can enter with: \nbash docker/scripts/dev_into.sh\n"
  printf "Enjoy!\n"
}

function main() {
  if [ "$#" -eq 0 ]; then
    show_usage
    exit 0
  fi

  local cmd="$1"
  case "${cmd}" in
  start)
    start_containers "$@"
    ;;
  stop)
    stop_containers "$@"
    ;;
  *)
    _usage
    ;;
  esac

}

main "$@"
