/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
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

#include "common/catch.hpp"
#include "scheduler/scheduler_policy.hpp"
int sum = 10;
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

TEST_CASE("Scheduler test") {
  // SchedulerConfig::Instance()->SetConfigPath("test_config.json");
  XEngine::Scheduler* scheduler = XEngine::Scheduler::Instance();
  scheduler->CreateTask("test_task", func_XEngine);
  scheduler->CreateTask("test_task2", func_XEngine2);
  while (count.load() < sum) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  REQUIRE(count.load() == sum);
  scheduler->CleanUp();
}
