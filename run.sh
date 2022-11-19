#!/usr/bin/env bash

function _usage() {
  echo -e "\n${RED}Usage${NO_COLOR}:
    .${BOLD}/run.sh${NO_COLOR} [OPTION]"
  echo -e "\n${RED}Options${NO_COLOR}:
    ${BLUE}build_opt [module]${NO_COLOR}: run optimized build.
    ${BLUE}run_benchmark_test${NO_COLOR}: run run_benchmark_test
    ${BLUE}compile_commands${NO_COLOR}: run compile_commands
    "
}

function build_opt() {
  # build all release
  #  --toolchain used clang++13
  xmake f --toolchain=clang -c && xmake
  compile_commands
}
function run_benchmark_test() {
  xmake run benchmark_test
}

function compile_commands() {
  xmake project -k compile_commands
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
  run_benchmark_test)
    run_benchmark_test "$@"
    ;;
  compile_commands)
    compile_commands "$@"
    ;;
  *)
    _usage
    ;;
  esac

}

main "$@"
