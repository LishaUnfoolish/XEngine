/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/22
@Desc: 信号与槽接口
***************************/

#pragma once

#include <atomic>
namespace XEngine {
class SpinLock {
 public:
  void lock() {
    while (flag_.test_and_set(std::memory_order_acquire)) { ; }
  }
  void unlock() { flag_.clear(std::memory_order_release); }

 private:
  std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};
}  // namespace XEngine