/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <type_traits>
#include <vector>

#include "common/macros.hpp"
namespace XEngine {
template <typename Matrix, typename Node>
class AdjacencyMatrix {
  using NodeContainer = std::conditional_t<std::is_void_v<Node>,
                                           std::false_type, std::vector<Node>>;

 public:
  using NodeType = Node;
  using EdgeType =
      std::decay_t<decltype(*std::declval<Matrix>().Row(0).begin())>;

  [[nodiscard]] constexpr NodeId Order() const noexcept {
    // The order of the graph
    // i.e. the number of vertices
    return static_cast<NodeId>(adjacency_matrix_.Rows());
  }
  [[nodiscard]] constexpr NodeId Size() const noexcept {
    // Number of sides
    return edges_sentinel_;
  }
  [[nodiscard]] constexpr void EdgeIncrease() noexcept { ++edges_sentinel_; }
  [[nodiscard]] constexpr void EdgeDecrease() noexcept { --edges_sentinel_; }
  [[nodiscard]] constexpr void ResetEdge() noexcept { edges_sentinel_ = 0; }

  [[nodiscard]] constexpr decltype(auto) GetNode(const NodeId& index) noexcept
      requires(!std::is_void_v<Node>) {
    return nodes.at(index);
  }
  [[nodiscard]] constexpr decltype(auto) GetNode(
      const NodeId& index) const noexcept requires(!std::is_void_v<Node>) {
    return nodes.at(index);
  }

  [[nodiscard]] constexpr auto OutEdges(const NodeId& index) noexcept {
    // The outgoing edge of the node
    return adjacency_matrix_.Row(index);
  }
  [[nodiscard]] constexpr auto OutEdges(const NodeId& index) const noexcept {
    return adjacency_matrix_.Row(index);
  }

 protected:
  Matrix adjacency_matrix_{};
  NodeId edges_sentinel_{0};
  NodeContainer nodes{};
};
}  // namespace XEngine