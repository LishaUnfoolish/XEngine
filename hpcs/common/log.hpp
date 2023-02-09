
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once

#include <stdarg.h>

#include <atomic>
#include <fstream>
#include <mutex>
#include <string>
#define LEFT_BRACKET "["
#define RIGHT_BRACKET "]"

#ifndef MODULE_NAME
#define MODULE_NAME ""
#endif

#define FILE_INFO __FILE__ << ":" << __LINE__ << " "
static inline std::atomic<std::ostream*> cout_{&std::cout};
#define DEBUG_MODULE(module)                                              \
  (*cout_.load()) << LEFT_BRACKET << module << RIGHT_BRACKET << FILE_INFO \
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
  (*cout_.load()) << LEFT_BRACKET << module << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_WARN(module) \
  (*cout_.load()) << LEFT_BRACKET << module << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_ERROR(module) \
  (*cout_.load()) << LEFT_BRACKET << module << RIGHT_BRACKET << FILE_INFO

#define LOG_MODULE_STREAM_FATAL(module) \
  (*cout_.load()) << LEFT_BRACKET << module << RIGHT_BRACKET << FILE_INFO
