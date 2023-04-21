#!/usr/bin/env bash
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@163.com
# @Time: 2022/11/20
# @Desc: 用户进入docker环境
# ***************************/
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../" && pwd -P)"
source ${ROOT_DIR}/docker/scripts/docker_common.sh

xhost +local:root 1>/dev/null 2>&1
docker exec \
    -u $USER \
    -it ${RUN_DEV} \
    /bin/bash
xhost -local:root 1>/dev/null 2>&1
