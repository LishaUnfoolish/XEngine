/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <list>
#include <vector>

namespace XEngine {

template <typename Edge, typename Node = void>
class AdjacencyList {
 public:
  using NodeType = Node;
  using NodeId = std::uint32_t;
  using EdgeType = Edge;
  using EdgeList = std::list<Edge>;
  using AdjacencyListType = std::vector<EdgeList>;

  NodeId AddNode(const NodeType& node) noexcept {
    if constexpr (!std::is_void_v<NodeType>) { nodes_.emplace_back(node); }
    adjacency_list_.emplace_back(EdgeList());
    return static_cast<NodeId>(adjacency_list_.size() - 1);
  }

  void AddEdge(NodeId from, NodeId to, const EdgeType& edge) noexcept {
    assert(from < adjacency_list_.size() && to < adjacency_list_.size());
    adjacency_list_[from].push_back(edge);
  }

  const EdgeList& OutEdges(NodeId node) const noexcept {
    assert(node < adjacency_list_.size());
    return adjacency_list_[node];
  }

  std::size_t Order() const noexcept { return adjacency_list_.size(); }

  NodeType& GetNode(NodeId id) noexcept {
    assert(id < nodes_.size());
    return nodes_[id];
  }

  const NodeType& GetNode(NodeId id) const noexcept {
    assert(id < nodes_.size());
    return nodes_[id];
  }

  std::vector<NodeId> TopologySort() const noexcept {
    std::vector<NodeId> sorted_nodes;
    std::vector<int> in_degree(adjacency_list_.size(), 0);
    for (const auto& edge_list : adjacency_list_) {
      for (const auto& edge : edge_list) { ++in_degree[edge.to]; }
    }
    std::vector<NodeId> queue;
    for (NodeId node = 0; node < adjacency_list_.size(); ++node) {
      if (in_degree[node] == 0) { queue.push_back(node); }
    }
    while (!queue.empty()) {
      NodeId node = queue.back();
      queue.pop_back();
      sorted_nodes.push_back(node);
      for (const auto& edge : adjacency_list_[node]) {
        --in_degree[edge.to];
        if (in_degree[edge.to] == 0) { queue.push_back(edge.to); }
      }
    }
    return sorted_nodes;
  }
  void RemoveNode(NodeId node) noexcept {
    assert(node < adjacency_list_.size());
    adjacency_list_.erase(std::next(adjacency_list_.begin(), node));
    if constexpr (!std::is_void_v<NodeType>) {
      nodes_.erase(std::next(nodes_.begin(), node));
    }
  }

  void RemoveEdge(NodeId from, NodeId to, const EdgeType& edge) noexcept {
    assert(from < adjacency_list_.size() && to < adjacency_list_.size());
    EdgeList& edge_list = adjacency_list_[from];
    edge_list.remove(edge);
  }

  void ClearEdges() noexcept {
    for (auto& edge_list : adjacency_list_) { edge_list.clear(); }
  }

 private:
  AdjacencyListType adjacency_list_;
  std::vector<NodeType> nodes_;
};

}  // namespace XEngine
