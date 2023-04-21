FROM lixiaoxmm/ubuntu22.10:original AS base

ENV LD_LIBRARY_PATH=/usr/local/lib
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

RUN  echo '\
  199.232.28.133 raw.githubusercontent.com \
  192.30.255.112 www.github.com \
  192.30.255.112 github.com \
  ' >> /etc/hosts


RUN apt-get update -yqq \
  && apt-get upgrade -yqq \
  && apt-get install -yqq --no-install-recommends \
  make \
  vim \
  pip \
  sudo \
  wget \
  curl \
  pkg-config \
  unzip \
  gnutls-bin \
  lsb-release \
  llvm \
  clang

COPY docker/ /docker
COPY docker/installers/tools /tools
RUN mkdir "/$(docker/scripts/docker_common.sh)"
RUN rm /docker -rf

