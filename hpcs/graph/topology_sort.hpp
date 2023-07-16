/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <cassert>
#include <queue>
#include <type_traits>

#include "hpcs/graph/adjacency_iterator.hpp"
namespace XEngine {
template <typename Dag>
class TopologySort {
 public:
  constexpr explicit TopologySort(const Dag& dag) noexcept(
      std::is_nothrow_default_constructible_v<Dag>) {
    NodeId node_size = dag.Order();
    sort_.resize(node_size, NodeIdNone);
    node_index_order.resize(node_size, NodeIdNone);
    std::vector<NodeId> node_indegrees(node_size, NodeIdNone);
    std::queue<NodeId> queue{};
    for (NodeId index = 0; index < node_size; index++) {
      /* find all indegree */
      node_indegrees[index] = dag.Indegree(index);
      /* find indegree is zero and push it to queue */
      if (dag.Indegree(index) == 0) { queue.push(index); }
    }
    for (NodeId i = 0; !queue.empty(); i++) {
      NodeId index = queue.front();
      queue.pop();
      sort_[i] = index;
      node_index_order[index] = i;
      auto iter = AdjacencyIterator(dag, index);
      for (; !iter.IsEnd(); ++iter) {
        if (0 == --node_indegrees[*iter]) { queue.push(*iter); }
      }
    }
  }

  // // Optimization 3.19 有问题
  // constexpr TopologySort(const Dag& dag) noexcept(
  //     std::is_nothrow_default_constructible_v<Dag>) {
  //   NodeId node_size = dag.Order();
  //   sort_.resize(node_size, NodeIdNone);
  //   node_index_order.resize(node_size);
  //   std::vector<NodeId> node_indegrees(node_size);
  //   std::queue<NodeId> queue{};
  //   for (NodeId index = 0; index < node_size; index++) {
  //     /* find all indegree /
  //     node_indegrees[index] = dag.Indegree(index);
  //     / find indegree is zero and push it to queue */
  //     if (node_indegrees[index] == 0) { queue.push(index); }
  //   }
  //   for (size_t i = 0; !queue.empty(); i++) {
  //     NodeId index = queue.front();
  //     queue.pop();
  //     sort_[i] = index;
  //     node_index_order[index] = i;
  //     auto iter = AdjacencyIterator(dag, index);
  //     for (; !iter.IsEnd(); ++iter) {
  //       if (0 == --node_indegrees[*iter]) { queue.push(*iter); }
  //     }
  //   }
  // }

  [[nodiscard]] constexpr NodeId GetNodeIdOrder(
      const NodeId& index) const noexcept {
    return node_index_order[index];
  }
  [[nodiscard]] constexpr const auto& operator()() const noexcept {
    return sort_;
  }

 private:
  std::vector<NodeId> sort_{};
  std::vector<NodeId> node_index_order{};
};

}  // namespace XEngine