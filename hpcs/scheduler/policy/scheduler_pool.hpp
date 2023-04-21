/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/singleton.hpp"
#include "scheduler/policy/pool_context.hpp"
#include "scheduler/processor.hpp"
#include "scheduler/scheduler.hpp"
#include "scheduler/scheduler_config.hpp"
namespace XEngine {
class SchedulerPool : public Scheduler {
 public:
  SchedulerPool() {
    auto* config = SchedulerConfig::Instance();
    if (config == nullptr) {
      ERROR << "SchedulerConfig is nullprt.\n";
      return;
    }

    const auto& task_attr = config->GetThreadPoolAttr();
    for (const auto& [name, attr] : task_attr) {
      auto ctx = std::make_shared<PoolContext>(name);
      auto proc = std::make_shared<Processor>();
      proc->BindContext(ctx);
      SchedulerConfig::SetThreadAttr(name, proc->Thread()->native_handle(),
                                     attr.policy, attr.priority, proc->Tid(),
                                     attr.cpuset);
      processors_.emplace_back(proc);
    }
  }

 public:
  bool CreateTask(const std::shared_ptr<Coroutine>& cr) override {
    if (stop_) [[unlikely]] {
        return true;
      }
    return PoolContext::Notify(cr);
  }

 private:
  bool NotifyProcessor(const std::shared_ptr<Coroutine>& cr) override {
    return false;
  }
};

}  // namespace XEngine