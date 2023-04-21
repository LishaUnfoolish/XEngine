

/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once
#include <semaphore.h>

#include <array>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <scheduler/processor.hpp>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/macros.hpp"
#include "core/semaphor.hpp"
#include "scheduler/croutine.hpp"
#include "scheduler/scheduler_config.hpp"

namespace XEngine {

class PoolContext : public Context {
 public:
  virtual ~PoolContext() { Shutdown(); }
  explicit PoolContext(std::string name) : name_(std::move(name)) {}
  void Shutdown() override {
    stop_.store(true);
    sem_.Stop();
  }

  std::shared_ptr<Coroutine> NextRoutine() override {
    if (stop_.load()) [[unlikely]] {
        return nullptr;
      }
    std::shared_ptr<Coroutine> cr;
    if (!private_queue_q.empty()) {
      cr = private_queue_q.front();
      private_queue_q.pop_front();
      return cr;
    }
    return nullptr;
  }

  static bool Notify(const std::shared_ptr<Coroutine>& cr) {
    {
      std::unique_lock<std::mutex> lk(PoolContext::mutex_);
      PoolContext::public_queue_q.emplace_back(cr);
    }
    PoolContext::sem_.Notify();
    return true;
  }

  void Wait() override {
    if (stop_) [[unlikely]] {
        return;
      }
    if (sem_.Wait(1000)) {
      // todo 需要做调度平衡和盗取？
      std::unique_lock<std::mutex> lk(PoolContext::mutex_);
      if (!PoolContext::public_queue_q.empty()) {
        private_queue_q.insert(
            private_queue_q.end(),
            std::make_move_iterator(PoolContext::public_queue_q.begin()),
            std::make_move_iterator(PoolContext::public_queue_q.begin() + 1));
        PoolContext::public_queue_q.erase(
            PoolContext::public_queue_q.begin(),
            PoolContext::public_queue_q.begin() + 1);
        // int size3 = private_queue_q.size();
        // if (size3 != 0) { DEBUG << size3 << std::endl; }
      }
    }
  }
  inline static Semaphore sem_{};
  inline static std::deque<std::shared_ptr<Coroutine>> public_queue_q{};
  inline static std::mutex mutex_{};

  std::deque<std::shared_ptr<Coroutine>> private_queue_q{};
  std::string name_{};
};
}  // namespace XEngine