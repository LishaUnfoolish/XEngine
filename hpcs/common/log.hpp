
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
            << "[DEBUG] "

// #define DEBUG_MODULE(module) std::ofstream{}
#define DEBUG DEBUG_MODULE(MODULE_NAME)
#define INFO LOG_MODULE(MODULE_NAME, INFO)
#define WARN LOG_MODULE(MODULE_NAME, WARN)
#define ERROR LOG_MODULE(MODULE_NAME, ERROR)
#define FATAL LOG_MODULE(MODULE_NAME, FATAL)

#ifndef LOG_MODULE_STREAM
#define LOG_MODULE_STREAM(log_severity) LOG_MODULE_STREAM_##log_severity
#endif

#ifndef LOG_MODULE
#define LOG_MODULE(module, log_severity) LOG_MODULE_STREAM(log_severity)(module)
#endif

#define LOG_MODULE_STREAM_INFO(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_WARN(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_ERROR(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_FATAL(module) \
  (std::cout) << LEFT_BRACKET << (module) << RIGHT_BRACKET << FILE_INFO
}  // namespace XEngine