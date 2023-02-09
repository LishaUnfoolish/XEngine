/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <optional>
#include <set>
#include <type_traits>

#include "graph/graph.hpp"
#include "graph/topology_sort.hpp"
struct Node {
  using Condition = int;
};

namespace XEngine {
template <typename Graph>
class FlowBuilder {
 public:
  using GraphType = Graph;
  using NodeType = typename GraphType::NodeType;
  using NodeIdType = typename GraphType::NodeIdType;

  [[nodiscard]] constexpr FlowBuilder(GraphType& graph) noexcept(
      std::is_nothrow_default_constructible_v<GraphType>)
      : graph_{graph} {}
  // Function: Emplace
  template <typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const Args&&... args) noexcept {
    return Emplace({std::forward<Args>(args)...}, deps);
  }

  template <typename Node, typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const Args&&... args) noexcept {
    return Emplace({std::forward<Args>(args)...}, deps);
  }

  template <typename Node, typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const std::string& name = "",
                                             const Args&&... args) noexcept {
    return Emplace({name, std::forward<Args>(args)...}, deps);
  }

  template <typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const std::string& name = "",
                                             const Args&&... args) noexcept {
    return Emplace({name, std::forward<Args>(args)...}, deps);
  }

  [[nodiscard]] constexpr std::optional<std::vector<NodeIdType>>
  Linearize() noexcept {
    return TopologySort<GraphType>{graph_}();
  }

  [[nodiscard]] constexpr GraphType& WorkFlows() noexcept { return graph_; }

 private:
  [[nodiscard]] constexpr NodeIdType Emplace(
      const NodeType& node, const std::set<NodeIdType>& deps) noexcept {
    NodeIdType node_index = graph_.AddNode(std::move(node));
    for (const auto& dep_index : deps) {
      graph_.AddEdge(dep_index, node_index);
    }
    return node_index;
  }

  GraphType& graph_;
};

}  // namespace XEngine
