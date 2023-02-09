#define CATCH_CONFIG_MAIN
#include <memory>

#include "common/catch.hpp"
#include "graph/dense_graph.hpp"
#include "graph/graph.hpp"
#include "task/flow_builder.hpp"
class BuilderTest {
 public:
  BuilderTest() = default;
  BuilderTest(const std::string& name) : name_(name) {}
  std::string Name() const { return name_; }

 private:
  std::string name_{};
};

TEST_CASE("test_flow_builder") {
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

  // graph.Graphviz();

  REQUIRE(graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 2);
  REQUIRE(graph.Outdegree(2) == 0);

  REQUIRE(graph.Indegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 1);

  graph.EraseNode(2);
  REQUIRE(!graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 1);
  REQUIRE(graph.Indegree(2) == 0);
}