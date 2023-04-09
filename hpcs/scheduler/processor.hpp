
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "common/log.hpp"
#include "scheduler/croutine.hpp"
#include "scheduler/processor.hpp"
namespace XEngine {

class Context {
 public:
  virtual ~Context() = default;
  virtual std::shared_ptr<Coroutine> NextRoutine() = 0;
  virtual void Shutdown() { stop_.store(true); }
  virtual void Wait() = 0;

 protected:
  std::atomic<bool> stop_{false};
};

class Processor {
 public:
  Processor() { running_.store(true); }
  Processor(const Processor&) = delete;
  Processor(Processor&&) = delete;
  Processor& operator=(const Processor&) = delete;
  Processor& operator=(Processor&&) = delete;
  virtual ~Processor() { Stop(); }
  std::thread* Thread() { return &thread_; }
  std::atomic<pid_t>& Tid() {
    pid_t value = -1;
    while (!tid_.compare_exchange_weak(value, -1)) {
      std::atomic_wait(&tid_, value);
      value = -1;
    }
    return tid_;
  }

  void Stop() {
    if (!running_.exchange(false)) [[unlikely]] {
        return;
      }
    if (context_) [[likely]] {
        context_->Shutdown();
      }
    cv_ctx_.notify_one();
    if (thread_.joinable()) [[likely]] {
        thread_.join();
      }
  }
  void Run() {
    /* 遍历每一个协程 */
    tid_.store(gettid());
    while (running_.load()) [[likely]] {
        if (context_ != nullptr) [[likely]] {
            auto croutine = context_->NextRoutine();
            if (croutine) [[likely]] {
                croutine->Resume();
                croutine->Release();
              }
            else {
              context_->Wait();
            }
          }
        else {
          std::unique_lock<std::mutex> lk(mtx_ctx_);
          cv_ctx_.wait_for(lk, std::chrono::milliseconds(10));
        }
      }
  }
  void BindContext(const std::shared_ptr<Context>& context) {
    context_ = context;
    std::call_once(thread_flag_,
                   [this]() { thread_ = std::thread(&Processor::Run, this); });
  }

 private:
  std::thread thread_;
  std::mutex mtx_ctx_;
  std::atomic<pid_t> tid_{-1};
  std::once_flag thread_flag_;
  std::condition_variable cv_ctx_;
  std::atomic<bool> running_{false};
  std::shared_ptr<Context> context_{};
};

}  // namespace XEngine
