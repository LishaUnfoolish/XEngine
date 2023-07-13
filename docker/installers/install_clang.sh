#!/usr/bin/env bash

# Fail on first error.
set -e

ROOT_DIR=/tmp/clang_toolchains
mkdir -p ${ROOT_DIR}

INSTALL_PREFIX=/usr/local

################
# llvm & clang #
################
cd ${ROOT_DIR}
git clone --branch llvmorg-15.0.7 --depth 1 https://gitee.com/openeuler/llvm-project.git

LLVM_BUILD_DIR="${ROOT_DIR}/llvm-project/build_ninja"
mkdir ${LLVM_BUILD_DIR}
cd ${LLVM_BUILD_DIR}

cmake \
  -GNinja \
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld;lldb" \
  -DLLVM_ENABLE_RUNTIMES="compiler-rt;libc;libcxx;libcxxabi;libunwind" \
  -DCMAKE_BUILD_TYPE="Release" \
  -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
  ../llvm

CLANG_LLVM_TARGETS=("clang" "clang-resource-headers" "runtimes" "builtins" "lld" "llvm-ar" "llvm-dwp" "llvm-cov" "llvm-nm" "llvm-objcopy" "llvm-objdump" "llvm-strip" "llvm-symbolizer" "lldb" "liblldb" "lldb-server" "clang-format" "clang-tidy")
ninja -j$(( $(nproc) - 2 ))
for target in ${CLANG_LLVM_TARGETS[@]}; do
  ninja install-${target}
done

ln -sf /usr/local/bin/clang-15 /usr/local/bin/clang-15.0.7
ln -sf /usr/local/bin/clang-15 /usr/local/bin/clang++-15.0.7
cd /tmp
rm -rf ${ROOT_DIR}
