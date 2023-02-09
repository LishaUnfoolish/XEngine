/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <algorithm>
#include <set>

#include "graph/adjacency_iterator.hpp"
#include "graph/graph_traits.hpp"
#include "graph/graph_writer.hpp"
namespace XEngine {

template <typename Impl>
class Graph : public Impl {
 public:
  using EdgeType = typename Impl::EdgeType;
  using NodeType = typename Impl::NodeType;
  using NodeIdType = NodeId;

  [[nodiscard]] constexpr std::uint32_t Outdegree(
      const NodeId& node_index) const noexcept {
    decltype(auto) outedge_iterator = Impl::OutEdges(node_index);
    return static_cast<std::uint32_t>(
        std::count_if(outedge_iterator.begin(), outedge_iterator.end(),
                      [](const EdgeType& edge) -> bool {
                        return Impl::placeholders != edge;
                      }));
  }

  [[nodiscard]] constexpr std::uint32_t Indegree(
      const NodeId& node_index) const noexcept {
    std::uint32_t degree{0};
    for (std::uint32_t index = 0; index < Impl::Order(); index++) {
      degree += static_cast<std::uint32_t>(Edges(index, node_index).Size());
    }
    return degree;
  }

  [[nodiscard]] constexpr auto Edges(const NodeId& from,
                                     const NodeId& to) const noexcept {
    auto first = std::next(Impl::OutEdges(from).begin(), to);
    if (*first == EdgeTraits<EdgeType>::placeholders) {
      return MakeRange(first, 0);
    } else {
      return MakeRange(first, 1);
    }
  }

  [[nodiscard]] constexpr bool HasEdge(const NodeId& from,
                                       const NodeId& to) const noexcept {
    return !Edges(from, to).Empty();
  }

  [[nodiscard]] constexpr void AddEdge(const NodeId& from, const NodeId& to,
                                       const EdgeType& edge) noexcept {
    Impl::AddEdge(from, std::next(Impl::OutEdges(from).begin(), to), edge);
  }

  template <typename Edge = EdgeType>
  [[nodiscard]] constexpr void AddEdge(const NodeId& from,
                                       const NodeId& to) noexcept
      requires(std::is_constructible_v<Edge, int>) {
    AddEdge(from, to, Edge{1});
  }

  [[nodiscard]] constexpr void Graphviz() const noexcept { ViewGraph(*this); }
};
}  // namespace XEngine