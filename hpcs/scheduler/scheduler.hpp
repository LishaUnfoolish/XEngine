
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
#include <memory>
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
  std::shared_ptr<Coroutine> CreateTask(std::string_view name, F&& fn,
                                        Args&&... args) {
    auto cr = std::make_shared<Coroutine>();
    cr->SetFunction(std::forward<F>(fn), std::forward<Args>(args)...);
    cr->SetId(std::hash<std::string_view>{}(name));
    cr->SetName(std::string(name));
    if (!CreateTask(cr)) { return nullptr; }
    return cr;
  }
  bool NotifyTask(const std::shared_ptr<Coroutine>& cr) {
    if (stop_.load()) [[unlikely]] {
        return true;
      }
    return NotifyProcessor(cr);
  }

 protected:
  Scheduler(const Scheduler&) = delete;
  Scheduler(Scheduler&&) = delete;
  Scheduler& operator=(const Scheduler&) = delete;
  Scheduler& operator=(Scheduler&&) = delete;
  virtual ~Scheduler() { CleanUp(); }
  Scheduler() : stop_(false) {}
  virtual bool NotifyProcessor(const std::shared_ptr<Coroutine>& cr) = 0;
  virtual bool CreateTask(const std::shared_ptr<Coroutine>& cr) = 0;
  void Shutdown() {
    stop_.exchange(true);
    for (auto& processor : processors_) { processor->Stop(); }
    processors_.clear();
  }

  std::atomic<bool> stop_{false};
  std::vector<std::shared_ptr<Processor>> processors_{};
  inline static std::atomic<Scheduler*> instance_ = {nullptr};
};

}  // namespace XEngine
