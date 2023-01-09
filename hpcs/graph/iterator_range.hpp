/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/12/05
@Desc: iterator range模板接口
***************************/
#pragma once
#include <algorithm>
namespace XEngine {
template <typename T>
class IteratorRange {
 public:
  using iterator = T;
  using const_iterator = std::add_const_t<iterator>;

  constexpr IteratorRange(const iterator& first, const iterator& last) noexcept(
      std::is_nothrow_default_constructible_v<iterator>)
      : first_(first), last_(last) {}
  constexpr IteratorRange(const iterator& first, std::uint32_t count) noexcept(
      std::is_nothrow_default_constructible_v<iterator>)
      : first_(first), last_(std::next(first, count)) {}
  IteratorRange() = default;
  IteratorRange(IteratorRange&&) = default;

  [[nodiscard]] constexpr void Reset(const iterator& first,
                                     const iterator& last) noexcept {
    first_ = first, last_ = last;
  }
  [[nodiscard]] constexpr bool operator==(
      const IteratorRange& rhs) const noexcept {
    return first_ == rhs.first_ && last_ == rhs.last_;
  }
  [[nodiscard]] constexpr bool operator!=(
      const IteratorRange& rhs) const noexcept {
    return first_ != rhs.first_ || last_ != rhs.last_;
  }

  // 作为容器使用
  [[nodiscard]] constexpr iterator begin() noexcept { return first_; }
  [[nodiscard]] constexpr iterator end() noexcept { return last_; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept {
    return first_;
  }
  [[nodiscard]] constexpr const_iterator end() const noexcept { return last_; }
  [[nodiscard]] constexpr auto Size() const noexcept {
    return std::distance(first_, last_);
  }
  [[nodiscard]] constexpr iterator& operator++() { return ++first_; }
  [[nodiscard]] constexpr void Advance(std::uint32_t n) {
    std::advance(first_, n);
  }
  [[nodiscard]] constexpr decltype(auto) operator*() { return *first_; }
  [[nodiscard]] constexpr decltype(auto) operator*() const { return *first_; }
  [[nodiscard]] constexpr bool Empty() const { return first_ == last_; }

 private:
  iterator first_{};
  iterator last_{};
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