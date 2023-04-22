/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/10
***************************/
#include <chrono>
#include <thread>
#define CATCH_CONFIG_MAIN
#include <atomic>
#include <functional>
#include <memory>

#include "hpcs/common/catch.hpp"
#include "hpcs/graph/dense_graph.hpp"
#include "hpcs/graph/graph.hpp"
#include "hpcs/task/flow_builder.hpp"
#include "hpcs/task/runner.hpp"
static std::atomic<int> test_num{0};
static std::atomic<int> sum{0};
class BuilderTest {
 public:
  BuilderTest() = default;
  BuilderTest(const std::string& name) : name_(name) {}
  std::string Name() const { return name_; }
  XEngine::RunnerStatus Run() {
    ++test_num;
    return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
  }

 private:
  std::string name_{};
};

TEST_CASE("test_runner") {
  XEngine::Graph<XEngine::DenseGraph<int, BuilderTest>> graph{};
  XEngine::FlowBuilder builder{graph};

  const auto& ret1 = builder.Emplace({}, "测试node1");
  REQUIRE(ret1 == 0);
  const auto& ret2 = builder.Emplace({}, "测试node2");
  REQUIRE(ret2 == 1);
  const auto& ret3 = builder.Emplace({ret2}, "测试node3");
  REQUIRE(ret3 == 2);
  const auto& ret4 = builder.Emplace({}, "测试node4");
  REQUIRE(ret4 == 3);
  const auto& ret5 = builder.Emplace({}, "测试node5");
  REQUIRE(ret5 == 4);
  const auto& ret6 = builder.Emplace({}, "测试node6");
  REQUIRE(ret6 == 5);
  const auto& ret7 = builder.Emplace({ret2}, "测试node7");
  REQUIRE(ret7 == 6);
  const auto& ret8 = builder.Emplace({}, "测试node8");
  REQUIRE(ret8 == 7);
  const auto& ret9 = builder.Emplace({}, "测试node9");
  REQUIRE(ret9 == 8);
  const auto& ret10 = builder.Emplace({}, "测试node10");
  REQUIRE(ret10 == 9);
  test_num = 0;
  REQUIRE(graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 2);
  REQUIRE(graph.Outdegree(2) == 0);

  REQUIRE(graph.Indegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 1);

  REQUIRE(test_num == 0);
  XEngine::Runner runner{builder};
  // graph.Graphviz();
  runner.Rebuild();
  runner.Run<XEngine::RunnerStatus>();
  REQUIRE(test_num == 10);

  graph.EraseNode(2);
  REQUIRE(!graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 1);
  REQUIRE(graph.Indegree(2) == 0);

  test_num = 0;
  // graph.Graphviz();
  runner.Rebuild();
  runner.Run<XEngine::RunnerStatus>();
  REQUIRE(test_num == 9);
}

XEngine::RunnerStatus test_function() {
  ++test_num;
  return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
}

TEST_CASE("test_function_runner") {
  XEngine::Graph<XEngine::DenseGraph<int, decltype(test_function)>> graph{};
  XEngine::FlowBuilder builder{graph};
  auto lambda_func = []() -> XEngine::RunnerStatus {
    ++test_num;
    return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
  };
  const auto& ret1 = builder.Emplace({}, "测试node1", test_function);
  REQUIRE(ret1 == 0);
  const auto& ret2 = builder.Emplace({}, "测试node2", test_function);
  REQUIRE(ret2 == 1);
  const auto& ret3 = builder.Emplace({ret2}, "测试node3", test_function);
  REQUIRE(ret3 == 2);
  const auto& ret4 = builder.Emplace({}, "测试node4", lambda_func);
  REQUIRE(ret4 == 3);
  const auto& ret5 = builder.Emplace({}, "测试node5", lambda_func);
  REQUIRE(ret5 == 4);
  const auto& ret6 = builder.Emplace({}, "测试node6", lambda_func);
  REQUIRE(ret6 == 5);
  const auto& ret7 = builder.Emplace({ret2}, "测试node7", lambda_func);
  REQUIRE(ret7 == 6);
  const auto& ret8 = builder.Emplace({}, "测试node8", lambda_func);
  REQUIRE(ret8 == 7);
  const auto& ret9 = builder.Emplace({}, "测试node9", lambda_func);
  REQUIRE(ret9 == 8);
  const auto& ret10 = builder.Emplace({}, "测试node10", lambda_func);
  REQUIRE(ret10 == 9);

  test_num = 0;
  REQUIRE(graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 2);
  REQUIRE(graph.Outdegree(2) == 0);

  REQUIRE(graph.Indegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 1);

  REQUIRE(test_num == 0);
  XEngine::Runner runner{builder};
  // graph.Graphviz();
  runner.Rebuild();
  runner.Run<XEngine::RunnerStatus>();
  REQUIRE(test_num == 10);

  graph.EraseNode(2);
  REQUIRE(!graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 1);
  REQUIRE(graph.Indegree(2) == 0);

  test_num = 0;
  // graph.Graphviz();
  runner.Rebuild();
  runner.Run<XEngine::RunnerStatus>();
  REQUIRE(test_num == 9);
}

XEngine::RunnerStatus test_args_function(const int& a, const int& b) {
  sum = a + b;
  ++test_num;
  return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
}
TEST_CASE("test_has_args_function_runner") {
  XEngine::Graph<XEngine::DenseGraph<int, decltype(test_args_function)>>
      graph{};
  XEngine::FlowBuilder builder{graph};
  auto lambda_func = [](const int& a, const int& b) -> XEngine::RunnerStatus {
    sum = a + b;
    ++test_num;
    return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
  };
  auto lambda_func2 = +[](const int& a, const int& b) -> XEngine::RunnerStatus {
    sum = a + b;
    ++test_num;
    return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
  };

  const auto& ret1 = builder.Emplace({}, "测试node1", lambda_func2);
  REQUIRE(ret1 == 0);
  const auto& ret2 = builder.Emplace({}, "测试node2", test_args_function);
  REQUIRE(ret2 == 1);
  const auto& ret3 = builder.Emplace({ret2}, "测试node3", test_args_function);
  REQUIRE(ret3 == 2);
  const auto& ret4 = builder.Emplace({}, "测试node4", lambda_func);
  REQUIRE(ret4 == 3);

  const auto& ret5 = builder.Emplace({}, "测试node5", test_args_function);
  REQUIRE(ret5 == 4);
  const auto& ret6 = builder.Emplace({}, "测试node6", lambda_func);
  REQUIRE(ret6 == 5);
  const auto& ret7 = builder.Emplace({ret2}, "测试node7", lambda_func);
  REQUIRE(ret7 == 6);
  const auto& ret8 = builder.Emplace({}, "测试node8", test_args_function);
  REQUIRE(ret8 == 7);
  const auto& ret9 = builder.Emplace({}, "测试node9", lambda_func2);
  REQUIRE(ret9 == 8);
  const auto& ret10 = builder.Emplace({}, "测试node10", lambda_func);
  REQUIRE(ret10 == 9);

  test_num = 0;
  sum = 0;
  REQUIRE(graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 2);
  REQUIRE(graph.Outdegree(2) == 0);

  REQUIRE(graph.Indegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 1);

  REQUIRE(test_num == 0);
  REQUIRE(sum == 0);
  XEngine::Runner runner{builder};
  // graph.Graphviz();
  const int& a = 1;
  const int& b = 2;
  runner.Rebuild();
  auto rrr = runner.Run(a, b);
  REQUIRE(test_num == 10);
  REQUIRE(sum == 3);

  graph.EraseNode(2);
  REQUIRE(!graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 1);
  REQUIRE(graph.Indegree(2) == 0);

  test_num = 0;
  sum = 0;
  // // graph.Graphviz();
  const int& c = 3;
  const int& d = 4;
  runner.Rebuild();
  runner.Run(c, d);
  REQUIRE(sum == 7);
  REQUIRE(test_num == 9);
}

TEST_CASE("test_has_args_function_runner_benchmark") {
  XEngine::Graph<XEngine::DenseGraph<int, XEngine::RunnerStatus()>> graph{};
  XEngine::FlowBuilder builder{graph};

  int count1 = 100;
  for (int i = count1; i > 0; i--) {
    std::set<XEngine::NodeId> dep;
    for (int k = 0; k < count1 - i; k++) { dep.insert(k); }
    auto ret = builder.Emplace(dep, "测试node10", []() {
      ++test_num;
      return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
    });
  }

  int count2 = 10000;
  for (int i = count2; i > 0; i--) {
    auto ret = builder.Emplace({}, "测试node10", [s = i]() {
      // std::this_thread::sleep_for(std::chrono::milliseconds(1));
      ++test_num;
      return XEngine::RunnerStatus{XEngine::RunnerStopReason::RunnerOk};
    });
  }

  XEngine::Runner runner{builder};
  graph.Graphviz("test_has_args_function_runner_benchmark.svg");
  int count3 = 1000;
  test_num = 0;
  runner.Rebuild();
  DEBUG << "start\n";
  std::chrono::time_point<std::chrono::steady_clock> start_time_ =
      std::chrono::steady_clock::now();
  // std::thread([] {
  //   while (true) {
  //     int in;
  //     // std::cin >> in;
  //     // 测试避免cpu占满导致其它模块响应不实时
  //     XEngine::SchedulerManager::Instance()->Submit([input = in]() {
  //       /* std::cout << "ssssssssss:" << input << std::endl;  */
  //     });
  //   }
  // }).detach();
  for (int i = count3; i > 0; i--) { runner.Run(); }
  DEBUG << std::chrono::steady_clock::now() - start_time_ << std::endl;
  REQUIRE(test_num == (count1 + count2) * count3);

  /* 因为Scheduler是个单例，所以得手动释放 */
  XEngine::SchedulerManager::Instance()->CleanUp();
  // 0-32-100 0.038 0.0018381  0.0048762  没有没延负载 ts:0.00101707秒.
  // 0-32-100 0.1439173 0.2176605  0.236163  milliseconds(1) ts:0.215866秒秒.
  // 0-100-100 0.2231402 0.7521842  0.7070208  milliseconds(1) ts:0.740608秒.
  // 0-10000-100 15.4600308 65.9998311  65.0809695 milliseconds(1) ts:65.944秒.
}