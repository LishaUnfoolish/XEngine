

/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/04/10
***************************/
#pragma once

#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "hpcs/common/macros.hpp"
#include "hpcs/core/semaphor.hpp"
#include "hpcs/scheduler/croutine.hpp"
#include "hpcs/scheduler/processor.hpp"
#include "hpcs/scheduler/scheduler_config.hpp"
namespace XEngine {

class PriorityContext : public Context {
 public:
  virtual ~PriorityContext() { Shutdown(); }
  explicit PriorityContext(const std::string& GroupName) {
    InitGroup(GroupName);
  }
  PriorityContext() {
    InitGroup(SchedulerConfig::Instance()->ProcessGroupName());
  }
  PriorityContext(const PriorityContext&) = delete;
  PriorityContext(PriorityContext&&) = delete;
  PriorityContext& operator=(const PriorityContext&) = delete;
  PriorityContext& operator=(PriorityContext&&) = delete;

  void Shutdown() override {
    stop_.store(true);
    for (int i = MAX_PRIO - 1; i >= 0; --i) {
      std::unique_lock<std::shared_mutex> lk(lock_que_->at(i));
      for (const auto& cr : policy_request_que_->at(i)) { Notify(cr); }
      sem_->Stop();
      policy_request_que_->at(i).clear();
    }
  }

  void InitGroup(const std::string& GroupName) {
    policy_request_que_ = &cr_group_[GroupName];
    lock_que_ = &rq_locks_[GroupName];
    sem_ = &sem_wq_[GroupName];
    { group_name_ = GroupName; }
  }

  std::shared_ptr<Coroutine> NextRoutine() override {
    if (stop_.load()) [[unlikely]] {
        return nullptr;
      }
    for (int i = MAX_PRIO - 1; i >= 0; --i) {
      // 每次都是最高优先级的任务先执行,如果高优先级任务一直都是READY，低优先级的任务会一直不触发
      std::shared_lock<std::shared_mutex> lk(lock_que_->at(i));
      for (auto& cr : policy_request_que_->at(i)) {
        if (!cr->Acquire()) [[unlikely]] {
            continue;
          }
        if (cr->State() == CoroutineState::READY) [[likely]] {
            return cr;
          }
        cr->Release();
      }
    }
    return nullptr;
  }

  static void Notify(const std::shared_ptr<Coroutine>& cr) {
    sem_wq_[cr->GroupName()].Notify();
  }

  void Wait() override {
    if (stop_) [[unlikely]] {
        return;
      }
    sem_->Wait(1000);
  }

  static bool RemoveCoroutine(const std::shared_ptr<Coroutine>& cr) {
    auto grp = cr->GroupName();
    auto prio = cr->Priority();
    auto crid = cr->Id();

    std::unique_lock<std::shared_mutex> lk(
        PriorityContext::rq_locks_[grp].at(prio));
    auto& croutines = PriorityContext::cr_group_[grp].at(prio);
    for (auto it = croutines.begin(); it != croutines.end(); ++it) {
      if ((*it)->Id() == crid) [[likely]] {
          auto cr = *it;
          cr->Stop();
          while (!cr->Acquire()) [[unlikely]] {
              std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
          croutines.erase(it);
          cr->Release();
          return true;
        }
    }
    return false;
  }

  using PriQueue =
      std::array<std::vector<std::shared_ptr<Coroutine>>, MAX_PRIO>;
  using CoroutineGroup = std::unordered_map<std::string, PriQueue>;
  using LockQueue = std::array<std::shared_mutex, MAX_PRIO>;
  using GroupLock = std::unordered_map<std::string, LockQueue>;
  using GroupSem = std::unordered_map<std::string, Semaphore>;

  alignas(hardware_constructive_interference_size) inline static CoroutineGroup
      cr_group_;
  alignas(hardware_constructive_interference_size) inline static GroupLock
      rq_locks_;
  alignas(
      hardware_constructive_interference_size) inline static GroupSem sem_wq_;

 private:
  PriQueue* policy_request_que_ = nullptr;
  LockQueue* lock_que_ = nullptr;
  std::string group_name_{};
  Semaphore* sem_{};
};
}  // namespace XEngine
