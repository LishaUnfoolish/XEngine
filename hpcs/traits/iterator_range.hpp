/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/12/05
@Desc: iterator range模板接口
***************************/
#pragma once
#include <utility>
namespace XEngine {

template <typename IteratorT>
class IteratorRange {
  IteratorT begin_iterator, end_iterator;

 public:
  template <typename Container>
  IteratorRange(Container &&c)
      // TODO: Consider ADL/non-member begin/end calls.
      : begin_iterator(c.begin()), end_iterator(c.end()) {}
  IteratorRange(IteratorT begin_iterator, IteratorT end_iterator)
      : begin_iterator(std::move(begin_iterator)),
        end_iterator(std::move(end_iterator)) {}

  IteratorT begin() const { return begin_iterator; }
  IteratorT end() const { return end_iterator; }
  bool empty() const { return begin_iterator == end_iterator; }
};

template <class T>
IteratorRange<T> MakeRange(T x, T y) {
  return IteratorRange<T>(std::move(x), std::move(y));
}

template <typename T>
IteratorRange<T> MakeRange(std::pair<T, T> p) {
  return IteratorRange<T>(std::move(p.first), std::move(p.second));
}

}  // namespace XEngine
