/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/12/05
@Desc: graph模板接口
***************************/
#pragma once

#include "traits/iterator_range.hpp"
namespace XEngine {
template <class GRAPH_TYPE>
struct GraphTraits {
  using NodeRef = typename GraphType::UnknownGraphTypeError;
  using GraphType = GRAPH_TYPE;
  using EdgeType = typename GraphType::EdgeType;
  using VertexType = typename GraphType::VertexType;
};

template <class GRAPH_TYPE>
struct Inverse {
  const GRAPH_TYPE &Graph;

  inline Inverse(const GRAPH_TYPE &G) : Graph(G) {}
};

template <class T>
struct GraphTraits<Inverse<Inverse<T>>> : GraphTraits<T> {};

template <class GRAPH_TYPE>
IteratorRange<typename GraphTraits<GRAPH_TYPE>::nodes_iterator> Nodes(
    const GRAPH_TYPE &G) {
  return MakeRange(GraphTraits<GRAPH_TYPE>::NodesBegin(G),
                   GraphTraits<GRAPH_TYPE>::NodesEnd(G));
}
template <class GRAPH_TYPE>
IteratorRange<typename GraphTraits<Inverse<GRAPH_TYPE>>::nodes_iterator>
InverseNodes(const GRAPH_TYPE &G) {
  return MakeRange(GraphTraits<Inverse<GRAPH_TYPE>>::NodesBegin(G),
                   GraphTraits<Inverse<GRAPH_TYPE>>::NodesEnd(G));
}

template <class GRAPH_TYPE>
IteratorRange<typename GraphTraits<GRAPH_TYPE>::ChildIteratorType> Children(
    const typename GraphTraits<GRAPH_TYPE>::NodeRef &G) {
  return MakeRange(GraphTraits<GRAPH_TYPE>::ChildBegin(G),
                   GraphTraits<GRAPH_TYPE>::ChildEnd(G));
}

template <class GRAPH_TYPE>
IteratorRange<typename GraphTraits<Inverse<GRAPH_TYPE>>::ChildIteratorType>
InverseChildren(const typename GraphTraits<GRAPH_TYPE>::NodeRef &G) {
  return MakeRange(GraphTraits<Inverse<GRAPH_TYPE>>::ChildBegin(G),
                   GraphTraits<Inverse<GRAPH_TYPE>>::ChildEnd(G));
}

template <class GRAPH_TYPE>
IteratorRange<typename GraphTraits<GRAPH_TYPE>::ChildEdgeIteratorType>
ChildrenEdges(const typename GraphTraits<GRAPH_TYPE>::NodeRef &G) {
  return MakeRange(GraphTraits<GRAPH_TYPE>::ChildEdgeBegin(G),
                   GraphTraits<GRAPH_TYPE>::ChildEdgeEnd(G));
}

}  // namespace XEngine
