/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <functional>
#include <optional>
#include <set>
#include <type_traits>

#include "hpcs/graph/graph.hpp"
#include "hpcs/graph/topology_sort.hpp"

namespace XEngine {
template <typename Graph>
class FlowBuilder {
 public:
  using GraphType = Graph;
  using NodeType = typename GraphType::NodeType;
  using NodeIdType = typename GraphType::NodeIdType;

  [[nodiscard]] constexpr explicit FlowBuilder(GraphType& graph) noexcept(
      std::is_nothrow_default_constructible_v<GraphType>)
      : graph_{graph} {}

  constexpr void Reserve(const std::uint32_t& node_count) const noexcept {
    return graph_.Reserve(node_count);
  }
  // Function: Emplace
  template <typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const Args&&... args) noexcept
      requires(!std::is_function_v<NodeType>) {
    return Emplace({std::forward<Args>(args)...}, deps);
  }

  template <typename Node, typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const Args&&... args) noexcept
      requires(!std::is_function_v<NodeType>) {
    return Emplace({std::forward<Args>(args)...}, deps);
  }

  template <typename Node, typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const std::string& name = "",
                                             const Args&&... args) noexcept
      requires(!std::is_function_v<NodeType>) {
    return Emplace({name, std::forward<Args>(args)...}, deps);
  }

  template <typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const std::string& name = "",
                                             const Args&&... args) noexcept
      requires(!std::is_function_v<NodeType>) {
    return Emplace({name, std::forward<Args>(args)...}, deps);
  }

  template <typename F, typename... Args>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const std::string& name, F&& f,
                                             Args&&... args) noexcept
      requires(std::is_function_v<NodeType>) {
    NodeIdType node_index = graph_.template AddNode<F, Args...>(
        std::forward<F>(f), std::forward<Args>(args)...);
    for (const auto& dep_index : deps) {
      graph_.AddEdge(dep_index, node_index);
    }
    return node_index;
  }

  template <typename T>
  [[nodiscard]] constexpr NodeIdType Emplace(const std::set<NodeIdType>& deps,
                                             const std::string& /*name*/,
                                             const T& lambda) noexcept
      requires(std::is_function_v<NodeType>) {
    NodeIdType node_index = graph_.AddNode(std::move(lambda));
    for (const auto& dep_index : deps) {
      graph_.AddEdge(dep_index, node_index);
    }
    return node_index;
  }

  [[nodiscard]] constexpr GraphType& WorkFlows() noexcept { return graph_; }
  [[nodiscard]] constexpr std::optional<std::vector<NodeIdType>>
  Linearize() noexcept {
    return TopologySort<GraphType>{graph_}();
  }

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