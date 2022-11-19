#!/usr/bin/env bash
xhost +local:root 1>/dev/null 2>&1
docker exec \
    -u $USER \
    -e HISTFILE=/XEngine/.dev_bash_hist \
    -it XEngine_$USER \
    /bin/bash
xhost -local:root 1>/dev/null 2>&1
