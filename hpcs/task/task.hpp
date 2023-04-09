/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <functional>
#include <future>
#include <memory>
#include <utility>

#include "common/singleton.hpp"
#include "concurrencpp/concurrencpp.h"

namespace XEngine {
#define USE_CONCUTTENCPP
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

#ifdef USE_CONCUTTENCPP

/* worker_thread_executor */
// template <typename F, typename... Args>
// [[nodiscard]] static constexpr auto Async(F&& f, Args&&... args)
//     -> concurrencpp::result<std::invoke_result_t<F, Args...>> {
//   return Singleton<concurrencpp::worker_thread_executor>::Instance()->submit(
//       std::bind(std::forward<F>(f), std::forward<Args>(args)...));
// }

/* thread_pool_executor */
struct Executor {
 public:
  Executor()
      : e("xengine_pool", std::thread::hardware_concurrency(),
          std::chrono::seconds(10)) {}
  concurrencpp::thread_pool_executor e;
};

template <typename F, typename... Args>
[[nodiscard]] static constexpr auto Async(F&& f, Args&&... args)
    -> concurrencpp::result<std::invoke_result_t<F, Args...>> {
  return Singleton<Executor>::Instance()->e.submit(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}

inline constexpr auto ReadyValue = concurrencpp::result_status::value;

template <typename T>
using RunnerFutureType = concurrencpp::result<T>;
#endif

}  // namespace XEngine