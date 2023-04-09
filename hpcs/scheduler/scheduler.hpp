
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/18
***************************/
#pragma once

#include <atomic>
#include <cstddef>
#include <functional>
#include <map>
#include <shared_mutex>
#include <string_view>
#include <thread>
#include <vector>

#include "scheduler/croutine.hpp"
#include "scheduler/processor.hpp"
#include "scheduler/scheduler_config.hpp"
namespace XEngine {

class Scheduler {
 public:
  static Scheduler* Instance();
  static void CleanUp();
  template <class F, class... Args>
  bool CreateTask(std::string_view name, F&& fn, Args&&... args) {
    auto cr = std::make_shared<Coroutine>();
    cr->SetFunction(std::forward<F>(fn), std::forward<Args>(args)...);
    cr->SetId(std::hash<std::string_view>{}(name));
    cr->SetName(std::string(name));
    return CreateTask(cr);
  }

 protected:
  Scheduler(const Scheduler&) = delete;
  Scheduler(Scheduler&&) = delete;
  Scheduler& operator=(const Scheduler&) = delete;
  Scheduler& operator=(Scheduler&&) = delete;
  virtual ~Scheduler() = default;
  Scheduler() : stop_(false) {}
  virtual bool CreateTask(const std::shared_ptr<Coroutine>& cr) = 0;
  void Shutdown() {
    if (stop_.exchange(true)) [[unlikely]] {
        return;
      }
    for (auto& processor : processors_) { processor->Stop(); }
    processors_.clear();
  }

  std::atomic<bool> stop_{false};
  std::vector<std::shared_ptr<Processor>> processors_{};
  inline static std::atomic<Scheduler*> instance_ = {nullptr};
};

}  // namespace XEngine
