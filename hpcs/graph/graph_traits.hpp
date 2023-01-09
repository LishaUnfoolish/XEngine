/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/12/05
@Desc: graph模板接口
***************************/
#pragma once

#include "graph/iterator_range.hpp"
namespace XEngine {

template <typename T>
struct GraphTraits {
  using GraphType = T;
  using EdgeType = typename T::EdgeType;
  using VertexType = typename T::VertexType;
  using EdgeRange = decltype(std::declval<T>().Outedges(0));
  using EdgeIterator = typename std::decay_t<EdgeRange>::iterator;
};

template <class T>
struct Inverse {
  const T &graph_;
  inline Inverse(const T &graph) : graph_(graph) {}
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

}  // namespace XEngine
