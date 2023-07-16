/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/02/29
***************************/
#pragma once
#include <functional>
#include <future>
#include <memory>
#include <utility>

#include "hpcs/common/singleton.hpp"
#include "hpcs/scheduler/scheduler_policy.hpp"
namespace XEngine {
#define USE_STD_ASYNC
#ifdef USE_STD_ASYNC
template <typename F, typename... Args>
[[nodiscard]] static constexpr auto Async(F&& f, Args&&... args)
    -> std::future<std::invoke_result_t<F, Args...>> {
  return std::async(std::launch::async,
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}
inline constexpr auto ReadyValue = std::future_status::ready;
template <typename T>
using RunnerFutureType = std::future<T>;
#endif

#ifdef USE_SCHEDULER

template <typename F, typename... Args>
[[nodiscard]] static constexpr auto Async(F&& f, Args&&... args)
    -> std::future<std::invoke_result_t<F, Args...>> {
  return SchedulerManager::Instance()->Submit(std::forward<F>(f),
                                              std::forward<Args>(args)...);
}
inline constexpr auto ReadyValue = std::future_status::ready;
template <typename T>
using RunnerFutureType = std::future<T>;
#endif
}  // namespace XEngine