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

  // [[nodiscard]] constexpr std::uint32_t Outdegree(
  //     const NodeId& node_index) const noexcept {
  //   decltype(auto) outedge_iterator = Impl::OutEdges(node_index);
  //   return static_cast<std::uint32_t>(
  //       std::count_if(outedge_iterator.begin(), outedge_iterator.end(),
  //                     [](const EdgeType& edge) -> bool {
  //                       return EdgeTraits<EdgeType>::placeholders != edge;
  //                     }));
  // }
  [[nodiscard]] constexpr std::uint32_t Outdegree(
      const NodeId& node_index) const noexcept {
    decltype(auto) outedge_iterator = Impl::OutEdges(node_index);
    std::uint32_t count = 0;
#pragma omp parallel for reduction(+ : count)
    for (std::size_t i = 0; i < outedge_iterator.Size(); ++i) {
      if (EdgeTraits<EdgeType>::placeholders != *outedge_iterator[i]) {
        ++count;
      }
    }
    return count;
  }

  // [[nodiscard]] constexpr std::uint32_t Indegree(
  //     const NodeId& node_index) const noexcept {
  //   decltype(auto) inedges_iterator = Impl::InEdges(node_index);
  //   return static_cast<std::uint32_t>(
  //       std::count_if(inedges_iterator.begin(), inedges_iterator.end(),
  //                     [](const EdgeType& edge) -> bool {
  //                       return EdgeTraits<EdgeType>::placeholders != edge;
  //                     }));
  // }

  [[nodiscard]] constexpr std::uint32_t Indegree(
      const NodeId& node_index) const noexcept {
    decltype(auto) inedges_iterator = Impl::InEdges(node_index);
    std::uint32_t count = 0;
#pragma omp parallel for reduction(+ : count)
    for (std::size_t i = 0; i < inedges_iterator.size(); ++i) {
      if (EdgeTraits<EdgeType>::placeholders != inedges_iterator[i]) {
        ++count;
      }
    }
    return count;
  }

  [[nodiscard]] constexpr EdgeType Edges(const NodeId& from,
                                         const NodeId& to) const noexcept {
    return *(Impl::OutEdges(from)[to]);
  }

  [[nodiscard]] constexpr bool HasEdge(const NodeId& from,
                                       const NodeId& to) const noexcept {
    return *(Impl::OutEdges(from)[to]) != EdgeTraits<EdgeType>::placeholders;
  }

  constexpr void AddEdge(const NodeId& from, const NodeId& to,
                         const EdgeType& edge) noexcept {
    Impl::AddEdge(from, std::next(Impl::OutEdges(from).begin(), to), edge);
  }

  template <typename Edge = EdgeType>
  constexpr void AddEdge(const NodeId& from, const NodeId& to) noexcept
      requires(std::is_constructible_v<Edge, int>) {
    AddEdge(from, to, Edge{1});
  }

  void Graphviz(const std::string file_path = "") const noexcept {
    ViewGraph(*this, file_path);
  }
};
}  // namespace XEngine