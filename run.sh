#!/usr/bin/env bash
# /***************************
# @Author: Xhosa-LEE
# @Contact: lixiaoxmm@gmail.com
# @Time: 2022/11/20
# @Desc: 编译运行脚本
# ***************************/
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/" && pwd -P)"
source /${ROOT_DIR}/docker/scripts/setup.bash
source /${ROOT_DIR}/docker/scripts/docker_common.sh
CONPILER="--config=clang --disk_cache=/${PROJECT_NAME}/.cache"
COMPILE_COMMANDS_JSON="compile_commands.json"
COMPILE_COMMANDS_FILES="//hpcs/..."
JOBS_THREAD=$(($(nproc) - 2))
function _usage() {
  echo -e "\n${RED}Usage${NO_COLOR}:
    .${BOLD}/run.sh${NO_COLOR} [OPTION]"
  echo -e "\n${RED}Options${NO_COLOR}:
    ${BLUE}build_opt [module]${NO_COLOR}: run optimized build.
    ${BLUE}build_dbg [module]${NO_COLOR}: run bdg build.
    ${BLUE}hpcs_benchmark${NO_COLOR}: run hpcs_benchmark
    ${BLUE}compile_commands${NO_COLOR}: run compile_commands
    ${BLUE}run_unit_opt_test${NO_COLOR}: run run_unit_opt_test
    ${BLUE}run_unit_dbg_test${NO_COLOR}: run run_unit_dbg_test
    ${BLUE}clang_tidy${NO_COLOR}: run clang_tidy
    "
}



function clang_tidy() {
  TIDY_FILE=${ROOT_DIR}/hpcs/
  compile_commands ${COMPILE_COMMANDS_FILES}
  if [ $? -ne 0 ]; then
    echo -e "${RED}compile_commands failed${NO_COLOR}"
    exit 1
  fi
  find ${TIDY_FILE} -name '*.cpp' -o -name '*.hpp' | xargs clang-tidy ${COMPILE_COMMANDS_JSON} --config-file=${ROOT_DIR}/.clang-tidy
}
function compile_commands() {
  if [ "$#" -eq 1 ]; then
    if [ -f ${ROOT_DIR}/${COMPILE_COMMANDS_JSON} ]; then
      mv ${ROOT_DIR}/${COMPILE_COMMANDS_JSON} ${ROOT_DIR}/${COMPILE_COMMANDS_JSON}_bak
    fi
    bazel-compdb -q $1
    if [ -f ${ROOT_DIR}/${COMPILE_COMMANDS_JSON}_bak ]; then
      mv ${ROOT_DIR}/${COMPILE_COMMANDS_JSON}_bak ${ROOT_DIR}/${COMPILE_COMMANDS_JSON}
    fi
  else
    bazel-compdb -q $COMPILE_COMMANDS_FILES
  fi

}
function build_opt() {
  compile_commands
  if [ $? -ne 0 ]; then
    echo -e "${RED}compile_commands failed${NO_COLOR}"
    exit 1
  fi
  # build all release
  bazel build $CONPILER ... --jobs=$JOBS_THREAD -c opt
  if [ $? -ne 0 ]; then
    echo -e "${RED}build failed${NO_COLOR}"
    exit 1
  fi
}

function build_dbg() {
  compile_commands
  if [ $? -ne 0 ]; then
    echo -e "${RED}compile_commands failed${NO_COLOR}"
    exit 1
  fi
  bazel build --subcommands --verbose_failures $CONPILER ... --jobs=$JOBS_THREAD -c dbg
  if [ $? -ne 0 ]; then
    echo -e "${RED}build failed${NO_COLOR}"
    exit 1
  fi
}

function hpcs_benchmark() {
  xmake run hpcs_benchmark
}

function run_unit_opt_test() {
  bazel test $CONPILER ... --jobs=$JOBS_THREAD -c opt
}

function run_unit_dbg_test() {
  bazel test $CONPILER ... --jobs=$JOBS_THREAD -c dbg
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
  run_unit_opt_test)
    run_unit_opt_test "$@"
    ;;
  run_unit_dbg_test)
    run_unit_dbg_test "$@"
    ;;
  clang_tidy)
    clang_tidy "$@"
    ;;
  *)
    _usage
    ;;
  esac

}

main "$@"
