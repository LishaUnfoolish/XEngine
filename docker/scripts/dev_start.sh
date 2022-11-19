#!/usr/bin/env bash 
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@163.com
# @Time: 2022/11/20
# @Desc: 部署、启动和运行docker
# ***************************/


DEV="lixiaoxmm/xengine:v1.3.xmake_1668767811"
IMG="dockerproxy.com/${DEV}" 
ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd -P )"


function show_usage () {
cat <<EOF
  Usage: $(basename $0) [options] ...
  OPTIONS:
      start                   star run containers.
      stop                   Stop all running containers.
EOF
  exit 0
}

function stop_containers()
{
running_containers=$(docker ps --format "{{.Names}}")

for i in ${running_containers[*]}
do
  if [[ "$i" =~ XEngine_* ]];then
    printf %-*s 70 "stopping container: $i ..."
    docker stop $i > /dev/null
    docker rm $i > /dev/null
    if [ $? -eq 0 ];then
      printf "\033[32m[DONE]\033[0m\n"
    else
      printf "\033[31m[FAILED]\033[0m\n"
    fi
  fi
done
}

function start_containers () {
  # start docker 
  sudo service docker start
  # pull
  docker pull $IMG
  if [ $? -ne 0 ];then
      printf "Failed to pull docker image."
      exit 1
  fi

  RUN_DEV="XEngine_${USER}"
  docker ps -a --format "{{.Names}}" | grep "$RUN_DEV" 1>/dev/null
  if [ $? == 0 ]; then
      if [[ "$(docker inspect --format='{{.Config.Image}}' $RUN_DEV 2> /dev/null)" != "$IMG" ]]; then
          rm -rf $ROOT_DIR/.xmake
          rm -rf $ROOT_DIR/build
      fi
      docker stop $RUN_DEV 1>/dev/null
      docker rm -v -f $RUN_DEV 1>/dev/null
  fi

  USER_ID=$(id -u)
  GRP=$(id -g -n)
  GRP_ID=$(id -g)
  LOCAL_HOST=`hostname`
  DOCKER_HOME="/home/$USER"
  if [ "$USER" == "root" ];then
      DOCKER_HOME="/root"
  fi
  if [ ! -d "$HOME/.cache" ];then
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
      -e DOCKER_IMG=$IMG \
      -v $ROOT_DIR:/XEngine\
      --net host \
      -w /XEngine\
      --add-host in_dev_docker:127.0.0.1 \
      --add-host ${LOCAL_HOST}:127.0.0.1 \
      --hostname in_dev_docker \
      --shm-size 2G \
      --pid=host \
      $IMG \
      /bin/bash
  if [ $? -ne 0 ];then
      printf "Failed to start docker container \"${RUN_DEV}\" based on image: $IMG"
      exit 1
  fi
  set +x

  if [ "${USER}" != "root" ]; then
      docker exec $RUN_DEV bash -c "/XEngine/docker/docker_adduser.sh"
  fi

  # tools xmake
  docker exec $RUN_DEV bash -c "cp /tools/.xmake/.* ${DOCKER_HOME}/ -R"

  printf "Finished setting up XEngine docker environment. Now you can enter with: \nbash docker/scripts/dev_into.sh\n"
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