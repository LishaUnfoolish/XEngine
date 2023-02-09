/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <future>
#include <memory>
#include <utility>

namespace XEngine {

template <typename F, typename... Args>
[[nodiscard]] static constexpr auto Async(F&& f, Args&&... args)
    -> std::future<std::invoke_result_t<F, Args...>> {
  return std::async(std::launch::async,
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}

}  // namespace XEngine