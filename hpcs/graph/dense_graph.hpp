/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/02/29
***************************/
#pragma once
#include <assert.h>

#include <concepts>
#include <functional>

#include "hpcs/graph/adjacency_matrix.hpp"
#include "hpcs/graph/graph_traits.hpp"
#include "hpcs/graph/matrix.hpp"
namespace XEngine {
template <typename Edge, typename Node = void>
class DenseGraph : public AdjacencyMatrix<Matrix<Edge>, Node> {
 public:
  using BaseT = AdjacencyMatrix<Matrix<Edge>, Node>;
  using NodeType = Node;
  using EdgeIterator = typename GraphTraits<BaseT>::EdgeIterator;
  virtual ~DenseGraph() noexcept = default;
  constexpr void Reserve(const std::uint32_t& node_count) noexcept {
    return BaseT::Reserve(node_count);
  }
  template <typename T>
  [[nodiscard]] constexpr NodeId AddNode(const T& node) noexcept
      requires(!std::is_void_v<T> && std::is_convertible_v<T, NodeType>) {
    BaseT::adjacency_matrix_.AppendRow(EdgeTraits<Edge>::placeholders);
    BaseT::adjacency_matrix_.AppendCol(EdgeTraits<Edge>::placeholders);
    BaseT::nodes.emplace_back(std::move(node));
    return static_cast<NodeId>(BaseT::nodes.size() - 1);
  }

  template <typename F, typename... Args>
  [[nodiscard]] constexpr NodeId AddNode(F&& func, Args&&... args) noexcept
      requires(std::is_function_v<NodeType>) {
    BaseT::adjacency_matrix_.AppendRow(EdgeTraits<Edge>::placeholders);
    BaseT::adjacency_matrix_.AppendCol(EdgeTraits<Edge>::placeholders);

    // const NodeId& id = static_cast<NodeId>(BaseT::nodes.size());
    // BaseT::nodes.emplace_back(std::move(Dispatcher<NodeId, Node>{})
    //                               .AddListener(id, std::forward<F>(func),
    //                                            std::forward<Args>(args)...));
    // return id;

    BaseT::nodes.emplace_back(
        std::move(std::bind<std::invoke_result_t<F, Args...>>(
            std::forward<F>(func), std::forward<Args>(args)...)));
    return static_cast<NodeId>(BaseT::nodes.size() - 1);
  }

  template <typename T>
  [[nodiscard]] constexpr NodeId AddNode(T&& lambda) noexcept
      requires(std::is_function_v<NodeType>) {
    return [this]<class R, class... Args>(std::function<R(Args...)> lambda)
        ->NodeId {
      BaseT::adjacency_matrix_.AppendRow(EdgeTraits<Edge>::placeholders);
      BaseT::adjacency_matrix_.AppendCol(EdgeTraits<Edge>::placeholders);
      BaseT::nodes.emplace_back(std::move(lambda));
      return static_cast<NodeId>(BaseT::nodes.size() - 1);
    }
    (std::move(std::function{std::move(lambda)}));
  }

  constexpr EdgeIterator AddEdge(NodeId from, EdgeIterator pos,
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

  constexpr void EraseNode(NodeId node) noexcept {
    if (!(node < BaseT::Order())) [[likely]] {
        assert(false && "Out of node index.");
      }
    if (!(BaseT::OutEdges(node).Count(EdgeTraits<Edge>::placeholders) ==
          BaseT::Order()))
      [[likely]] { assert(false && "Has edges is already initialized."); }

    BaseT::adjacency_matrix_.EraseRow(node);
    BaseT::adjacency_matrix_.EraseCol(node);
    if constexpr (!std::is_void_v<NodeType>) {
      BaseT::nodes.erase(std::next(BaseT::nodes.begin(), node));
    }
  }

  constexpr void EraseEdges() noexcept { assert(false); /* todo */ }
};

}  // namespace XEngine