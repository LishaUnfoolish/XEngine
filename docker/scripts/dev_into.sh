#!/usr/bin/env bash
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@163.com
# @Time: 2022/11/20
# @Desc: 用户进入docker环境
# ***************************/
xhost +local:root 1>/dev/null 2>&1
docker exec \
    -u $USER \
    -e HISTFILE=/XEngine/.dev_bash_hist \
    -it XEngine_$USER \
    /bin/bash
xhost -local:root 1>/dev/null 2>&1
