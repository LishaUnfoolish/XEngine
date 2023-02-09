/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <assert.h>

#include <concepts>

#include "graph/adjacency_matrix.hpp"
#include "graph/graph_traits.hpp"
#include "graph/matrix.hpp"
namespace XEngine {
template <typename Edge, typename Node = void>
class DenseGraph : public AdjacencyMatrix<Matrix<Edge>, Node> {
 public:
  using BaseT = AdjacencyMatrix<Matrix<Edge>, Node>;
  using NodeType = Node;
  using EdgeIterator = typename GraphTraits<BaseT>::EdgeIterator;

  template <typename T>
  [[nodiscard]] constexpr NodeId AddNode(const T& node) noexcept
      requires(!std::is_void_v<T> && std::is_convertible_v<T, NodeType>) {
    BaseT::adjacency_matrix_.AppendRow(EdgeTraits<Edge>::placeholders);
    BaseT::adjacency_matrix_.AppendCol(EdgeTraits<Edge>::placeholders);
    BaseT::nodes.push_back(NodeType(node));
    return static_cast<NodeId>(BaseT::nodes.size() - 1);
  }
  [[nodiscard]] constexpr EdgeIterator AddEdge(NodeId from, EdgeIterator pos,
                                               const Edge& edge) noexcept {
    if (!(from < BaseT::Order())) [[likely]] {
        assert(false && "Out of node index.");
      }
    if (!(pos >= BaseT::OutEdges(from).begin() &&
          pos <= BaseT::OutEdges(from).end()))
      [[likely]] { assert(false && "Edge is no find."); }
    if (*pos != EdgeTraits<Edge>::placeholders) [[likely]] {
        assert(false && "Edge Existing.");
      }
    *pos = edge;
    BaseT::EdgeIncrease();
    return pos;
  }

  [[nodiscard]] constexpr void EraseNode(NodeId node) noexcept {
    if (!(node < BaseT::Order())) [[likely]] {
        assert(false && "Out of node index.");
      }
    if (!(BaseT::OutEdges(node).Count(EdgeTraits<Edge>::placeholders) ==
          BaseT::Order()))
      [[likely]] { assert(false && "Has edges is already initialized."); }

    BaseT::adjacency_matrix_.EraseRow(node),
        BaseT::adjacency_matrix_.EraseCol(node);
    if constexpr (!std::is_void_v<NodeType>) {
      BaseT::nodes.erase(std::next(BaseT::nodes.begin(), node));
    }
  }

  [[nodiscard]] constexpr void EraseEdges() noexcept {
    assert(false); /* todo */
  }
};

}  // namespace XEngine