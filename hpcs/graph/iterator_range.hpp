/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/01/10
@Desc: 迭代器接口
***************************/
#pragma once
#include <algorithm>
namespace XEngine {
template <typename T>
class IteratorRange {
 public:
  using Iterator = T;
  using ConstIterator = std::add_const_t<Iterator>;

  constexpr IteratorRange(const Iterator& first, const Iterator& last) noexcept(
      std::is_nothrow_default_constructible_v<Iterator>)
      : first_(first), last_(last) {}
  constexpr IteratorRange(const Iterator& first, std::uint32_t count) noexcept(
      std::is_nothrow_default_constructible_v<Iterator>)
      : first_(first), last_(std::next(first, count)) {}
  IteratorRange() = default;
  IteratorRange(IteratorRange&&) = default;

  [[nodiscard]] constexpr void Reset(const Iterator& first,
                                     const Iterator& last) noexcept {
    first_ = first;
    last_ = last;
  }
  [[nodiscard]] constexpr bool operator==(
      const IteratorRange& rhs) const noexcept {
    return first_ == rhs.first_ && last_ == rhs.last_;
  }
  [[nodiscard]] constexpr bool operator!=(
      const IteratorRange& rhs) const noexcept {
    return first_ != rhs.first_ || last_ != rhs.last_;
  }

  // iterator range
  [[nodiscard]] constexpr Iterator begin() noexcept { return first_; }
  [[nodiscard]] constexpr Iterator end() noexcept { return last_; }
  [[nodiscard]] constexpr ConstIterator begin() const noexcept {
    return first_;
  }
  [[nodiscard]] constexpr ConstIterator end() const noexcept { return last_; }
  [[nodiscard]] constexpr auto Size() const noexcept {
    return std::distance(first_, last_);
  }
  [[nodiscard]] constexpr Iterator& operator++() { return ++first_; }
  [[nodiscard]] constexpr void Advance(std::uint32_t n) {
    std::advance(first_, n);
  }
  [[nodiscard]] constexpr decltype(auto) operator*() { return *first_; }
  [[nodiscard]] constexpr decltype(auto) operator*() const { return *first_; }
  [[nodiscard]] constexpr bool Empty() const { return first_ == last_; }

 private:
  Iterator first_{};
  Iterator last_{};
};

template <class T>
IteratorRange<T> MakeRange(T x, std::uint32_t count) {
  return IteratorRange<T>(std::move(x), count);
}

template <class T>
IteratorRange<T> MakeRange(T x, T y) {
  return IteratorRange<T>(std::move(x), std::move(y));
}

template <typename T>
IteratorRange<T> MakeRange(std::pair<T, T> p) {
  return IteratorRange<T>(std::move(p.first), std::move(p.second));
}

}  // namespace XEngine