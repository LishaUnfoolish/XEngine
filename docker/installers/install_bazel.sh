#!/usr/bin/env bash
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../" && pwd -P)"
source ${ROOT_DIR}/docker/scripts/docker_common.sh
source ${ROOT_DIR}/docker/installers/installer_base.sh

set -e
TARGET_ARCH=$(uname -m)
BAZEL_VERSION="3.7.1"
BUILDTOOLS_VERSION="3.5.0"

if [[ "$TARGET_ARCH" == "x86_64" ]]; then
  # https://docs.bazel.build/versions/master/install-ubuntu.html
  PKG_NAME="bazel_${BAZEL_VERSION}-linux-x86_64.deb"
  DOWNLOAD_LINK="https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/${PKG_NAME}"
  SHA256SUM="2c6c68c23618ac3f37c73ba111f79212b33968217e1a293aa9bf5a17cdd3212b"
  download_if_not_cached $PKG_NAME $SHA256SUM $DOWNLOAD_LINK
  apt_get_update_and_install \
    g++ \
    zlib1g-dev
  # https://docs.bazel.build/versions/master/install-ubuntu.html#step-3-install-a-jdk-optional
  # openjdk-11-jdk
  dpkg -i "${PKG_NAME}"
  # Cleanup right after installation
  rm -rf "${PKG_NAME}"
  ## buildifier ##
  PKG_NAME="buildifier-${BUILDTOOLS_VERSION}.${TARGET_ARCH}.bin"
  CHECKSUM="f9a9c082b8190b9260fce2986aeba02a25d41c00178855a1425e1ce6f1169843"
  DOWNLOAD_LINK="https://github.com/bazelbuild/buildtools/releases/download/${BUILDTOOLS_VERSION}/buildifier"
  download_if_not_cached "${PKG_NAME}" "${CHECKSUM}" "${DOWNLOAD_LINK}"
  cp -f ${PKG_NAME} "${SYSROOT_DIR}/bin/buildifier"
  chmod a+x "${SYSROOT_DIR}/bin/buildifier"
  rm -rf ${PKG_NAME}
  info "Done installing bazel ${BAZEL_VERSION} with buildifier ${BUILDTOOLS_VERSION}"
else
  error "Target arch ${TARGET_ARCH} not supported yet"
  exit 1
fi
apt-get clean && \
    rm -rf /var/lib/apt/lists/*