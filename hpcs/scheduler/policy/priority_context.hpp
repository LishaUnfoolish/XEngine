

/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once
#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <scheduler/processor.hpp>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "common/macros.hpp"
#include "scheduler/croutine.hpp"
#include "scheduler/scheduler_config.hpp"
namespace XEngine {
static constexpr uint32_t MAX_PRIO = 20;
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
      std::shared_lock<std::shared_mutex> lk(lock_que_->at(i));
      policy_request_que_->at(i).clear();
    }
  }

  void InitGroup(const std::string& GroupName) {
    policy_request_que_ = &cr_group_[GroupName];
    lock_que_ = &rq_locks_[GroupName];
    condition_variable_ = &cv_wq_[GroupName];
    {
      std::scoped_lock lk(mtx_wq_);
      notify_grp_[GroupName] = 0;
      group_name_ = GroupName;
    }
  }

  std::shared_ptr<Coroutine> NextRoutine() override {
    if (stop_.load()) { return nullptr; }
    for (int i = MAX_PRIO - 1; i >= 0; --i) {
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

  static void Notify(const std::string& GroupName) {
    {
      std::scoped_lock lk(mtx_wq_);
      ++notify_grp_[GroupName];
    }
    cv_wq_[GroupName].notify_one();
  }

  void Wait() override {
    std::unique_lock<std::mutex> lk(mtx_wq_);
    condition_variable_->wait_for(lk, std::chrono::milliseconds(1000), [&]() {
      return notify_grp_[group_name_] > 0;
    });
    if (notify_grp_[group_name_] > 0) [[likely]] {
        notify_grp_[group_name_]--;
      }
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
  using GroupCv = std::unordered_map<std::string, std::condition_variable>;
  using GroupNotify = std::unordered_map<std::string, int>;

  alignas(hardware_constructive_interference_size) inline static CoroutineGroup
      cr_group_;
  alignas(hardware_constructive_interference_size) inline static GroupLock
      rq_locks_;
  alignas(
      hardware_constructive_interference_size) inline static std::mutex mtx_wq_;
  alignas(hardware_constructive_interference_size) inline static GroupNotify
      notify_grp_;
  alignas(hardware_constructive_interference_size) inline static GroupCv cv_wq_;

 private:
  PriQueue* policy_request_que_ = nullptr;
  LockQueue* lock_que_ = nullptr;
  std::condition_variable* condition_variable_ = nullptr;
  std::string group_name_{};
};
}  // namespace XEngine
