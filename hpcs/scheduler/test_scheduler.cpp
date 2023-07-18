/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/03/10
***************************/
#define CATCH_CONFIG_MAIN
#include <algorithm>
#include <atomic>
#include <chrono>
#include <coroutine>
#include <deque>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "hpcs/common/catch.hpp"
#include "hpcs/scheduler/scheduler_policy.hpp"
int sum = 3;
std::atomic<int> count{0};

XEngine::Future<XEngine::CoroutineState> func_XEngine() {
  ++count;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  co_return XEngine::CoroutineState::FINISHED;
}

XEngine::Future<XEngine::CoroutineState> func_XEngine2() {
  while (count.load() < sum) {
    ++count;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    co_yield XEngine::CoroutineState::READY;
  }
  co_return XEngine::CoroutineState::FINISHED;
}
TEST_CASE("Scheduler scheduler config") {
  auto* config_ptr = XEngine::SchedulerConfig::Instance();
  config_ptr->SetConfigPath("/XEngine/conf/test.json");
  XEngine::SchedulingPolicy policy = config_ptr->GetPolicy();
  XDEBUG << (int)policy << std::endl;
}

TEST_CASE("Scheduler test") {
  // SchedulerConfig::Instance()->SetConfigPath("test_config.json");
  XEngine::Scheduler* scheduler = XEngine::Scheduler::Instance();
  scheduler->CreateTask("test_task", func_XEngine);
  scheduler->CreateTask("test_task2", func_XEngine2);
  while (count.load() < sum) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  REQUIRE(count.load() == sum);
  /* 因为Scheduler是个单例，所以得手动释放 */
  scheduler->CleanUp();
}

template <typename F, typename... Args>
auto Submit(F&& func, Args&&... args)
    -> std::future<std::invoke_result_t<F, Args...>> {
  using return_type = std::invoke_result_t<F, Args...>;
  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(func), std::forward<Args>(args)...));
  (*task)();
  return task->get_future();
}
TEST_CASE("test Submit") {
  XEngine::Scheduler* scheduler = XEngine::Scheduler::Instance();
  std::atomic<int> fun1_count = 5;
  std::atomic<int> fun2_count = 5;
  auto func1 = [&fun1_count]() { --fun1_count; };
  auto func2 = [&fun2_count]() { --fun2_count; };
  while (fun1_count > 0 && fun2_count > 0) {
    std::future<void> ret1 =
        XEngine::SchedulerManager::Instance()->Submit(func1);
    std::future<void> ret2 = Submit(func2);
    while (
        std::future_status::ready != ret1.wait_for(std::chrono::seconds(1)) ||
        std::future_status::ready != ret2.wait_for(std::chrono::seconds(1))) {
      ret1.get();
      ret2.get();
    }
    REQUIRE(fun1_count.load() == fun2_count.load());
  }
  scheduler->CleanUp();
}