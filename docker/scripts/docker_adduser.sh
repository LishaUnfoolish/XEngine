#!/usr/bin/env bash
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@gmail.com
# @Time: 2022/11/20
# @Desc: 部署docker环境时添加使用者的用户环境
# ***************************/
addgroup --gid "$DOCKER_GRP_ID" "$DOCKER_GRP"
adduser --disabled-password --force-badname --gecos '' "$DOCKER_USER" --uid "$DOCKER_USER_ID" --gid "$DOCKER_GRP_ID" 2>/dev/null
usermod -aG sudo "$DOCKER_USER"
echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
echo '
ulimit -c unlimited
source /usr/local/lib/bazel/bin/bazel-complete.bash
' >> "/home/${DOCKER_USER}/.bashrc"

echo '
genhtml_branch_coverage = 1
lcov_branch_coverage = 1
' > "/home/${DOCKER_USER}/.lcovrc"

# Set user files ownership to current user, such as .bashrc, .profile, etc.
chown ${DOCKER_USER}:${DOCKER_GRP} /home/${DOCKER_USER}
ls -ad /home/${DOCKER_USER}/.??* | xargs chown -R ${DOCKER_USER}:${DOCKER_GRP}

# add user
# useradd -d /home/$DOCKER_USER -s /bin/bash -m $DOCKER_USER
echo
echo "$DOCKER_USER:123456" | chpasswd
echo
echo "User $DOCKER_USER's password has been changed!"