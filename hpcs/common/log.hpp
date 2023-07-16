
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once

#include <atomic>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace XEngine {
#define LEFT_BRACKET "["
#define RIGHT_BRACKET "]"

#ifndef MODULE_NAME
#define MODULE_NAME ""
#endif

#define FILE_INFO __FILE__ << ":" << __LINE__ << " "
#define DEBUG_MODULE(module)                                          \
  std::cout << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO \
            << "[XDEBUG] "

// #define DEBUG_MODULE(module) std::ofstream{}
#define XDEBUG DEBUG_MODULE(MODULE_NAME)
#define XINFO LOG_MODULE(MODULE_NAME, XINFO)
#define XWARN LOG_MODULE(MODULE_NAME, XWARN)
#define XERROR LOG_MODULE(MODULE_NAME, XERROR)
#define XFATAL LOG_MODULE(MODULE_NAME, XFATAL)

#define LOG_MODULE_STREAM(log_severity) LOG_MODULE_STREAM_##log_severity

#define LOG_MODULE(module, log_severity) LOG_MODULE_STREAM(log_severity)(module)

#define LOG_MODULE_STREAM_XINFO(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_XWARN(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_XERROR(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_XFATAL(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO
}  // namespace XEngine