/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once

#include <cassert>

#include "scheduler/policy/scheduler_priority.hpp"
#include "scheduler/scheduler.hpp"
#include "scheduler/scheduler_config.hpp"
namespace XEngine {
Scheduler* Scheduler::Instance() {
  Scheduler* obj = Scheduler::instance_.load(std::memory_order_acquire);
  if (obj == nullptr) [[unlikely]] {
      obj = Scheduler::instance_.load(std::memory_order_relaxed);
      if (obj == nullptr) {
        SchedulingPolicy policy = SchedulerConfig::Instance()->GetPolicy();
        if (policy == SchedulingPolicy::PRIORITY) [[unlikely]] {
            obj = new SchedulerPriority();
          }
        else {
          ERROR << "No found " << (int)policy << ", use default param.\n";
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
