/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <assert.h>

#include <queue>

#include "graph/adjacency_iterator.hpp"
namespace XEngine {
class Bool {
 public:
  constexpr Bool() noexcept : value_() {}
  constexpr Bool(bool value) noexcept : value_(value) {}
  [[nodiscard]] constexpr operator bool() const noexcept { return value_; }
  [[nodiscard]] constexpr bool* operator&() noexcept { return &value_; }
  [[nodiscard]] constexpr const bool* const operator&() const noexcept {
    return &value_;
  }

 private:
  bool value_;
};

template <typename Graph, bool FindEdge = false>
class BfIterator {
 public:
  using EdgeType = typename Graph::EdgeType;
  using NodeIdType = typename Graph::NodeIdType;
  using AdjIter = AdjacencyIterator<Graph>;
  using constEdgeRef = decltype(std::declval<AdjIter>().Edge());

  constexpr BfIterator(
      const Graph& graph,
      const NodeIdType&
          start_node) noexcept(std::is_nothrow_default_constructible_v<Graph>)
      : graph_(graph),
        node_start_(NodeIdNone),
        is_visited_(graph.Order(), false) {
    if (start_node != NodeIdNone) { Start(start_node); }
  }

  [[nodiscard]] constexpr decltype(auto) operator++() noexcept {
    ToNext();
    return *this;
  }
  [[nodiscard]] constexpr NodeIdType operator*() const noexcept {
    return visit_queue_.empty() ? node_start_ : *(visit_queue_.front());
  }
  [[nodiscard]] constexpr NodeIdType From() const noexcept {
    assert(!IsEnd());
    return visit_queue_.empty() ? NodeIdNone : visit_queue_.front().From();
  }
  [[nodiscard]] constexpr constEdgeRef Edge() const noexcept {
    return visit_queue_.front().Edge();
  }
  [[nodiscard]] constexpr bool IsEnd() const noexcept {
    return node_start_ == NodeIdNone;
  }
  [[nodiscard]] constexpr void Start(const NodeIdType& node) noexcept {
    assert(IsEnd() && !is_visited_[node]);
    node_start_ = node;
    if (FindEdge) {
      ++(*this);  // skip start node
    }
  }
  [[nodiscard]] constexpr void ToNext() noexcept {
    assert(!IsEnd());
    if (!is_visited_[node_start_]) {
      assert(visit_queue_.empty());
      is_visited_[node_start_] = true;
      visit_queue_.push(AdjIter(graph_, node_start_));
    } else {
      assert(!visit_queue_.empty());
      auto& iter = visit_queue_.front();
      NodeIdType node = *iter;
      if (!is_visited_[node]) {
        is_visited_[node] = true;
        visit_queue_.push(AdjIter(graph_, node));
      }
      ++iter;
    }
    while (!visit_queue_.empty()) {
      auto& iter = visit_queue_.front();
      if (iter.IsEnd()) {
        visit_queue_.pop();
        continue;
      }
      if (!FindEdge) {
        if (is_visited_[*iter]) {
          ++iter;
          continue;
        }
      }
      break;
    }
    if (visit_queue_.empty()) { node_start_ = NodeIdNone; }
    if (IsEnd()) {
      auto pos = std::find(is_visited_.begin(), is_visited_.end(), false);
      if (pos != is_visited_.end()) {
        Start(static_cast<NodeIdType>(std::distance(is_visited_.begin(), pos)));
      }
    }
  }

 private:
  const Graph& graph_;
  std::queue<AdjIter> visit_queue_{};
  NodeIdType node_start_{};
  std::vector<Bool> is_visited_{};
};
}  // namespace XEngine