/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <assert.h>

#include "common/macros.hpp"
#include "graph/graph_traits.hpp"
namespace XEngine {
template <typename Graph>
class AdjacencyIterator {
 public:
  using EdgeRange = typename GraphTraits<Graph>::EdgeRange;
  using EdgeType = typename Graph::EdgeType;
  using NodeType = typename Graph::NodeType;

  constexpr explicit AdjacencyIterator(Graph& graph) noexcept(
      std::is_nothrow_default_constructible_v<Graph>)
      : graph_(graph), from_(NodeIdNone) {}
  constexpr AdjacencyIterator(Graph& graph, const NodeId& node) noexcept(
      std::is_nothrow_default_constructible_v<Graph>)
      : graph_(graph), range_(graph.OutEdges(node)), from_(node) {
    // skip null Edge
    if (*range_ == EdgeTraits<EdgeType>::placeholders) { ++(*this); }
  }
  [[nodiscard]] constexpr NodeId From() const noexcept { return from_; }
  [[nodiscard]] constexpr NodeId To() const noexcept {
    return static_cast<NodeId>(
        std::distance(graph_.OutEdges(from_).begin(), range_.begin()));
  }
  [[nodiscard]] constexpr NodeId operator*() const noexcept { return To(); }
  [[nodiscard]] constexpr void operator++() noexcept {
    assert(!IsEnd());
    ++range_;
    while (!IsEnd() && *range_ == EdgeTraits<EdgeType>::placeholders) {
      ++range_;
    }
  }
  [[nodiscard]] constexpr bool IsEnd() const noexcept { return range_.Empty(); }
  // get the value of current Edge
  [[nodiscard]] constexpr const EdgeType& Edge() const noexcept {
    return *range_;
  }

 protected:
  Graph& graph_{};
  EdgeRange range_{};
  NodeId from_{};
};
}  // namespace XEngine