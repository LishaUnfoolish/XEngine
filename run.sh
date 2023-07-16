#!/usr/bin/env bash
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@163.com
# @Time: 2022/11/20
# @Desc: 编译运行脚本
# ***************************/
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/" && pwd -P)"
function _usage() {
  echo -e "\n${RED}Usage${NO_COLOR}:
    .${BOLD}/run.sh${NO_COLOR} [OPTION]"
  echo -e "\n${RED}Options${NO_COLOR}:
    ${BLUE}build_opt [module]${NO_COLOR}: run optimized build.
    ${BLUE}build_dbg [module]${NO_COLOR}: run bdg build.
    ${BLUE}hpcs_benchmark${NO_COLOR}: run hpcs_benchmark
    ${BLUE}compile_commands${NO_COLOR}: run compile_commands
    ${BLUE}run_unit_test${NO_COLOR}: run unit_test
    "
}

function build_opt() {
  # build all release
  bazel build --config=clang ... --jobs=$(($(nproc) - 2)) -c opt
}

function build_dbg() {
  bazel build --config=clang ... --jobs=$(($(nproc) - 2)) -c dbg
}

function hpcs_benchmark() {
  xmake run hpcs_benchmark
}

function compile_commands() {
  ${ROOT_DIR}/scripts/gtage.sh
  # xmake project -k compile_commands
}
function unit_test() {
  target=(
    "hpcs_benchmark"
    "test_flow_builder"
    "test_graph"
    "test_runner"
    "test_signal"
  )
  for i in ${target[@]}; do
    xmake run $i
  done
}

function main() {
  if [ "$#" -eq 0 ]; then
    _usage
    exit 0
  fi

  local cmd="$1"
  case "${cmd}" in
  build_opt)
    build_opt "$@"
    ;;
  build_dbg)
    build_dbg "$@"
    ;;
  hpcs_benchmark)
    hpcs_benchmark "$@"
    ;;
  compile_commands)
    compile_commands "$@"
    ;;
  run_unit_test)
    unit_test "$@"
    ;;
  *)
    _usage
    ;;
  esac

}

main "$@"
