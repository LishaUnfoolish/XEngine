/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/10
***************************/
#define CATCH_CONFIG_MAIN
#include <memory>

#include "graph/dense_graph.hpp"
#include "graph/graph.hpp"
#include "hpcs/common/catch.hpp"

class Base {
 public:
  virtual std::string Name() { return "Base"; }
};
class Data : public Base {
 public:
  Data(const std::string& name) : name_(name) {}
  std::string Name() override { return name_; }
  std::string name_{};
};

class EdgeTest {
 public:
  bool operator!=(const EdgeTest& in) const { return in.i != i; }
  bool operator==(const EdgeTest& in) const { return in.i == i; }
  int operator()() const { return i; }
  int i = 0;
};

TEST_CASE("test_graph_virtual_functions") {
  XEngine::Graph<XEngine::DenseGraph<int, std::shared_ptr<Base>>> graph{};
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_1")) == 0);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_2")) == 1);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_3")) == 2);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_4")) == 3);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_5")) == 4);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_6")) == 5);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_7")) == 6);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_8")) == 7);

  REQUIRE(!graph.HasEdge(2, 1));
  graph.AddEdge(1, 2);
  REQUIRE(graph.HasEdge(1, 2));

  REQUIRE(graph.Outdegree(1) == 1);
  REQUIRE(graph.Outdegree(2) == 0);

  REQUIRE(graph.Indegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 1);

  graph.EraseNode(2);
  REQUIRE(!graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 0);
}

TEST_CASE("test_graph_edge") {
  XEngine::Graph<XEngine::DenseGraph<EdgeTest, std::shared_ptr<Base>>> graph{};
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_1")) == 0);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_2")) == 1);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_3")) == 2);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_4")) == 3);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_5")) == 4);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_6")) == 5);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_7")) == 6);
  REQUIRE(graph.AddNode(std::make_shared<Data>("test_data_8")) == 7);

  REQUIRE(!graph.HasEdge(2, 1));
  graph.AddEdge(1, 2, EdgeTest{123});
  REQUIRE(graph.HasEdge(1, 2));

  REQUIRE(graph.Edges(1, 2) == EdgeTest{123});

  REQUIRE(graph.Outdegree(1) == 1);
  REQUIRE(graph.Outdegree(2) == 0);

  REQUIRE(graph.Indegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 1);

  graph.EraseNode(2);
  REQUIRE(!graph.HasEdge(1, 2));
  REQUIRE(graph.Outdegree(1) == 0);
  REQUIRE(graph.Indegree(2) == 0);
}