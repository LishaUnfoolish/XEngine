
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/04/10
***************************/
#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "hpcs/common/log.hpp"
#include "hpcs/common/singleton.hpp"
#include "hpcs/scheduler/policy/priority_context.hpp"
#include "hpcs/scheduler/processor.hpp"
#include "hpcs/scheduler/scheduler.hpp"
#include "hpcs/scheduler/scheduler_config.hpp"
/* 这是一个优先级调度器，优先保证高优先级的任务执行，牺牲了高并发性保证可靠性 */
namespace XEngine {
class SchedulerPriority : public Scheduler {
  friend Scheduler* Scheduler::Instance();
  SchedulerPriority() {
    auto* config = SchedulerConfig::Instance();
    if (config == nullptr) {
      XERROR << "SchedulerConfig is nullprt.\n";
      return;
    }

    const auto& group_map = config->GetCoroutineGroup();
    for (const auto& [GroupName, group] : group_map) {
      auto proc_num = group.processor_num;
      for (uint32_t i = 0; i < proc_num; i++) {
        auto ctx = std::make_shared<PriorityContext>(GroupName);
        auto proc = std::make_shared<Processor>();
        proc->BindContext(ctx);

        decltype(group.attr.cpuset) cput_set =
            (i >= group.attr.cpuset.size())
                ? decltype(group.attr.cpuset){}
                : decltype(group.attr.cpuset){group.attr.cpuset[i]};
        SchedulerConfig::SetThreadAttr(
            GroupName, proc->Thread()->native_handle(), group.attr.policy,
            group.attr.priority, proc->Tid(), cput_set);
        processors_.emplace_back(proc);
      }
    }
  }

 public:
  bool CreateTask(const std::shared_ptr<Coroutine>& cr) override {
    auto* config = SchedulerConfig::Instance();
    if (config == nullptr) {
      XERROR << "SchedulerConfig is nullprt.\n";
      return false;
    }
    /*
      1.这里是根据配置文件来和创建的协程属性进行匹配，
      如果配置文件配置了创建的协程，则用配置文件的属性进行配置，如果没有配置则使用默认属性。
      2.因为还没有添加动态配置，所以全部都先采用默认配置
     */
    const auto& task_info_map = config->GetTask();
    const auto& task_iter = task_info_map.find(cr->Name());
    if (task_iter != task_info_map.end()) {
      cr->SetPriority(task_iter->second.priority);
      cr->SetGroupName(task_iter->second.GroupName);
    } else {
      // 确保至少存在一个group
      cr->SetGroupName(task_info_map.begin()->second.GroupName);
    }
    /* 做防御，进程优先级最大不超过MAX_PRIO */
    if (cr->Priority() >= MAX_PRIO) {
      XWARN << cr->Name() << " prio is greater than MAX_PRIO[ << " << MAX_PRIO
           << "].";
      cr->SetPriority(MAX_PRIO - 1);
    }

    // Submit task.
    {
      std::unique_lock<std::shared_mutex> lk(
          PriorityContext::rq_locks_[cr->GroupName()].at(cr->Priority()));
      PriorityContext::cr_group_[cr->GroupName()]
          .at(cr->Priority())
          .emplace_back(cr);
    }

    PriorityContext::Notify(cr);
    return true;
  }

 private:
  bool NotifyProcessor(const std::shared_ptr<Coroutine>& cr) override {
    if (stop_) [[unlikely]] {
        return true;
      }
    PriorityContext::Notify(cr);
    return true;
  }
};

}  // namespace XEngine
