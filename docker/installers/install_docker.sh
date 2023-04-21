#!/usr/bin/env bash
function install() {
MACHINE_ARCH=$(uname -m)
if [ "$MACHINE_ARCH" == 'x86_64' ]; then
  sudo add-apt-repository \
     "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
elif [ "$MACHINE_ARCH" == 'aarch64' ]; then
  sudo bash -c 'echo "deb [arch=arm64] https://download.docker.com/linux/ubuntu xenial edge" > /etc/apt/sources.list.d/docker.list'
else
  echo "Unknown machine architecture $MACHINE_ARCH"
  exit 1
fi
  sudo apt-get update
  sudo apt-get install -y docker-ce
  sudo groupadd docker
  sudo gpasswd -a "$USER" docker
  sudo usermod -aG docker $USER
  newgrp docker

  sudo curl -L "https://github.com/docker/compose/releases/download/2.17.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
  sudo chmod +x /usr/local/bin/docker-compose
}

case $1 in
  install)
      install
      ;;
   uninstall)
      sudo apt-get remove docker docker-engine docker.io
      sudo apt-get purge docker-ce
      rm -rf /usr/local/bin/docker-compose
      ;;
   *)
      echo "error."
esac
