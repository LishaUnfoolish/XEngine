/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

namespace XEngine {

template <typename Edge, typename Node = void>
class ChainedForwardStar {
 public:
  using NodeType = Node;
  using NodeId = std::uint32_t;
  using EdgeType = Edge;

  struct EdgeNode {
    EdgeType edge;
    NodeId to;
    int next;
  };

  NodeId AddNode(const NodeType& node) noexcept {
    if constexpr (!std::is_void_v<NodeType>) { nodes_.emplace_back(node); }
    head_.push_back(-1);
    return static_cast<NodeId>(head_.size() - 1);
  }

  void AddEdge(NodeId from, NodeId to, const EdgeType& edge) noexcept {
    assert(from < head_.size() && to < head_.size());
    EdgeNode edge_node{edge, to, head_[from]};
    edges_.push_back(edge_node);
    head_[from] = static_cast<int>(edges_.size() - 1);
  }

  template <typename F>
  void TraverseOutEdges(NodeId node, F&& func) const {
    assert(node < head_.size());
    for (int i = head_[node]; i != -1; i = edges_[i].next) {
      func(edges_[i].edge, edges_[i].to);
    }
  }

  std::size_t Order() const noexcept { return head_.size(); }

  NodeType& GetNode(NodeId id) noexcept {
    assert(id < nodes_.size());
    return nodes_[id];
  }

  const NodeType& GetNode(NodeId id) const noexcept {
    assert(id < nodes_.size());
    return nodes_[id];
  }

 private:
  std::vector<int> head_;
  std::vector<EdgeNode> edges_;
  std::vector<NodeType> nodes_;
};

}  // namespace XEngine
