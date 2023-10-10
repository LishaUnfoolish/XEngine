#!/usr/bin/env bash
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../" && pwd -P)"
# https://blog.csdn.net/weixin_42888638/article/details/127144788
#install nvidia-docker2

bash $ROOT_DIR/docker/host/install_nvidia_docker.sh

# install cuda
wget https://developer.download.nvidia.com/compute/cuda/11.7.0/local_installers/cuda_11.7.0_515.43.04_linux.run
sudo sh cuda_11.7.0_515.43.04_linux.run
# 出现界面输入:accept
# 再出现界面按空格取消安装Driver,然后选择install回车安装
# 设置环境变量:
#              export PATH=/usr/local/cuda-11.7/bin${PATH:+:${PATH}}
#              export LD_LIBRARY_PATH=/usr/local/cuda-11.7/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
#source ~/.bashrc
#nvcc --version
#docker run --gpus all -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix nvcr.io/nvidia/k8s/cuda-sample:nbody nbody -gpu -benchmark

######带图像输出测试
# apt-get update && apt-get install -y xvfb
# Xvfb :1 -screen 0 1024x768x16 &
# export DISPLAY=:1