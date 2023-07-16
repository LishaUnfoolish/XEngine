/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/01/10
@Desc: graph模板接口
***************************/
#pragma once

#include "hpcs/graph/iterator_range.hpp"
namespace XEngine {

template <typename T>
struct GraphTraits {
  using GraphType = T;
  using EdgeType = typename T::EdgeType;
  using NodeType = typename T::NodeType;
  using EdgeRange = decltype(std::declval<T>().OutEdges(0));
  using EdgeIterator = typename std::decay_t<EdgeRange>::Iterator;
};
template <typename Type>
struct EdgeTraits {
  using EdgeType = Type;
  static constexpr const EdgeType placeholders = {};
};

template <typename Type>
struct NodeTraits {
  using NodeType = Type;
};

template <class T>
struct Inverse {
  const T& graph_;
  inline Inverse(const T& graph) : graph_(graph) {}
};

template <class T>
struct GraphTraits<Inverse<Inverse<T>>> : GraphTraits<T> {};

// template <class GraphType>
// IteratorRange<typename GraphTraits<GraphType>::nodes_iterator> Nodes(
//     const GraphType &G) {
//   return MakeRange(GraphTraits<GraphType>::NodesBegin(G),
//                    GraphTraits<GraphType>::NodesEnd(G));
// }
// template <class GraphType>
// IteratorRange<typename GraphTraits<Inverse<GraphType>>::nodes_iterator>
// InverseNodes(const GraphType &G) {
//   return MakeRange(GraphTraits<Inverse<GraphType>>::NodesBegin(G),
//                    GraphTraits<Inverse<GraphType>>::NodesEnd(G));
// }

// template <class GraphType>
// IteratorRange<typename GraphTraits<GraphType>::ChildIteratorType> Children(
//     const typename GraphTraits<GraphType>::NodeRef &G) {
//   return MakeRange(GraphTraits<GraphType>::ChildBegin(G),
//                    GraphTraits<GraphType>::ChildEnd(G));
// }

// template <class GraphType>
// IteratorRange<typename GraphTraits<Inverse<GraphType>>::ChildIteratorType>
// InverseChildren(const typename GraphTraits<GraphType>::NodeRef &G) {
//   return MakeRange(GraphTraits<Inverse<GraphType>>::ChildBegin(G),
//                    GraphTraits<Inverse<GraphType>>::ChildEnd(G));
// }

// template <class GraphType>
// IteratorRange<typename GraphTraits<GraphType>::ChildEdgeIteratorType>
// ChildrenEdges(const typename GraphTraits<GraphType>::NodeRef &G) {
//   return MakeRange(GraphTraits<GraphType>::ChildEdgeBegin(G),
//                    GraphTraits<GraphType>::ChildEdgeEnd(G));
// }
//

}  // namespace XEngine
