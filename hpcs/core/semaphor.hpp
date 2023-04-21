/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/22
@Desc: 信号与槽接口
***************************/

#pragma once
#include <semaphore.h>

#include <atomic>
#include <chrono>
#include <semaphore>
#include <stdexcept>
namespace XEngine {
class Semaphore {
 public:
  Semaphore() = default;
  ~Semaphore() { Stop(); }
  void Stop() {
    stop_.exchange(true);
    while (wait_count_.load() > 0) { Notify(); }
  }
  bool Wait(int timeout_ms) {
    if (stop_.load()) { return false; }
    wait_count_.fetch_add(1);
    return semaphore_.try_acquire_until(std::chrono::steady_clock::now() +
                                        std::chrono::milliseconds(timeout_ms));
  }

  void Notify() {
    semaphore_.release();
    wait_count_.fetch_sub(1);
  }

 private:
  std::binary_semaphore semaphore_{0};
  std::atomic_bool stop_{false};
  std::atomic_int wait_count_{0};
};

}  // namespace XEngine