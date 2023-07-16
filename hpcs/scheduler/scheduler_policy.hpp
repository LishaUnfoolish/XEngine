/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <deque>
#include <future>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "hpcs/common/singleton.hpp"
#include "hpcs/core/spin_lock.hpp"
#include "hpcs/scheduler/policy/scheduler_pool.hpp"
#include "hpcs/scheduler/policy/scheduler_priority.hpp"
#include "hpcs/scheduler/scheduler.hpp"
#include "hpcs/scheduler/scheduler_config.hpp"
#include "hpcs/scheduler/scheduler_policy.hpp"
namespace XEngine {
class SchedulerManager {
 public:
  SchedulerManager() {}
  virtual ~SchedulerManager() { Shutdown(); }
  void Shutdown() {
    stop_.exchange(true);
    thread_pool_.CleanUp();
    Scheduler::Instance()->CleanUp();
  }

  template <typename F, typename... Args>
  auto Submit(F&& func, Args&&... args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    using return_type = std::invoke_result_t<F, Args...>;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(func), std::forward<Args>(args)...));
    if (!stop_.load()) [[likely]] {
        auto cr = std::make_shared<Coroutine>();
        cr->SetFunction([task]() -> XEngine::Future<XEngine::CoroutineState> {
          (*task)();
          co_return XEngine::CoroutineState::FINISHED;
        });
        thread_pool_.CreateTask(cr);
      }
    return task->get_future();
  }

 private:
  std::atomic<bool> stop_ = {false};
  SchedulerPool thread_pool_{};
  DECLARE_SINGLETON(SchedulerManager)
};

Scheduler* Scheduler::Instance() {
  Scheduler* obj = Scheduler::instance_.load(std::memory_order_acquire);
  if (obj == nullptr) [[unlikely]] {
      obj = Scheduler::instance_.load(std::memory_order_relaxed);
      if (obj == nullptr) {
        if (!XEngine::SchedulerConfig::AffinityCpuset(
                pthread_self(),
                SchedulerConfig::Instance()->GetProcessAffinityCpuset())) {
          XERROR << "Failed to set affinity.\n";
        }
        SchedulingPolicy policy = SchedulerConfig::Instance()->GetPolicy();
        if (policy == SchedulingPolicy::PRIORITY) [[unlikely]] {
            obj = new SchedulerPriority();
          }
        else {
          XERROR << "No found " << (int)policy << ", use default param.\n";
          assert(false && "policy no find.");
        }

        Scheduler::instance_.store(obj, std::memory_order_release);
      }
    }
  return obj;
}

inline void Scheduler::CleanUp() {
  Scheduler* obj = Scheduler::instance_.load(std::memory_order_acquire);
  if (obj != nullptr) { obj->Shutdown(); }
}

}  // namespace XEngine
