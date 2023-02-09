/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/01/10
***************************/
#pragma once
#include <assert.h>

#include <vector>

#include "common/macros.hpp"
#include "graph/iterator_range.hpp"
namespace XEngine {
template <typename T>
class Matrix {
 public:
  using Container = std::vector<T>;
  using ConstReference = typename Container::const_reference;
  Matrix() = default;
  constexpr Matrix(
      const NodeId& rows, const NodeId& cols,
      ConstReference
          init_val) noexcept(std::is_nothrow_default_constructible_v<T>)
      : rows_(rows), cols_(cols) {
    data_.assign(rows * cols, init_val);
  }

  [[nodiscard]] constexpr void Resize(const NodeId& rows, const NodeId& cols,
                                      const ConstReference& init_val) noexcept {
    rows_ = rows;
    cols_ = cols;
    data_.assign(rows * cols, init_val);
  }

  [[nodiscard]] constexpr void Reserve(const NodeId& rows,
                                       const NodeId& cols) noexcept {
    data_.Reserve(rows * cols);
  }
  [[nodiscard]] constexpr NodeId Rows() const noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    return rows_;
  }
  [[nodiscard]] constexpr NodeId Cols() const noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    return cols_;
  }
  [[nodiscard]] constexpr void Insert(const NodeId& row, const NodeId& col,
                                      const ConstReference& value) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    data_[row * cols_ + col] = value;
  }
  // Returns the range of iterations for the index row
  [[nodiscard]] constexpr auto Row(const NodeId& index) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    auto start = std::next(data_.begin(), index * cols_);
    return MakeRange(start, cols_);
  }
  [[nodiscard]] constexpr auto Row(const NodeId& index) const noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    auto start = std::next(data_.cbegin(), index * cols_);
    return MakeRange(start, cols_);
  }
  [[nodiscard]] constexpr void EraseRow(const NodeId& index) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    auto range = Row(index);
    data_.erase(range.begin(), range.end());
    --rows_;
  }
  [[nodiscard]] constexpr void EraseCol(const NodeId& index) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    --cols_;
    auto iter = std::next(data_.begin(), index);
    for (NodeId r = 0; r < rows_; r++) {
      iter = data_.erase(iter);
      if (r != rows_ - 1) { std::advance(iter, cols_); }
    }
  }
  [[nodiscard]] constexpr void AppendRow(
      const ConstReference& init_val) noexcept {
    data_.resize(data_.size() + cols_, init_val);
    ++rows_;
  }
  [[nodiscard]] constexpr void AppendCol(
      const ConstReference& init_val) noexcept {
    data_.resize(data_.size() + rows_);
    auto src = data_.begin() + (rows_ - 1) * cols_;
    auto dst = data_.end() - 1;
    // The first line does not move, so r starts from 1
    for (NodeId r = 1; r < rows_; r++) {
      std::copy_backward(src, src + cols_, dst);
      *dst = init_val;
      src -= cols_;
      dst -= cols_ + 1;
    }
    // Assign a value to the new column in the first row
    *(src + cols_) = init_val;
    ++cols_;
  }

 private:
  NodeId rows_{0};
  NodeId cols_{0};
  Container data_{};
};
}  // namespace XEngine