/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/01/10
@Desc: æÿ’Û
***************************/
#pragma once
#include <assert.h>

#include <vector>

#include "graph/iterator_range.hpp"
namespace XEngine {
template <typename T>
class Matrix {
 public:
  using Container = std::vector<T>;
  using ConstReference = typename Container::const_reference;
  Matrix() = default;
  constexpr Matrix(
      std::uint32_t rows, std::uint32_t cols,
      ConstReference
          init_val) noexcept(std::is_nothrow_default_constructible_v<T>)
      : rows_(rows), cols_(cols) {
    data_.assign(rows * cols, init_val);
  }

  [[nodiscard]] constexpr void Resize(std::uint32_t rows, std::uint32_t cols,
                                      ConstReference init_val) noexcept {
    rows_ = rows;
    cols_ = cols;
    data_.assign(rows * cols, init_val);
  }

  [[nodiscard]] constexpr void Reserve(std::uint32_t rows,
                                       std::uint32_t cols) noexcept {
    data_.Reserve(rows * cols);
  }
  [[nodiscard]] constexpr std::uint32_t Rows() const noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    return rows_;
  }
  [[nodiscard]] constexpr std::uint32_t Cols() const noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    return cols_;
  }
  [[nodiscard]] constexpr void Insert(std::uint32_t row, std::uint32_t col,
                                      ConstReference val) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    data_[row * cols_ + col] = val;
  }
  // Returns the range of iterations for the index row
  [[nodiscard]] constexpr auto Row(std::uint32_t index) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    auto start = std::next(data_.begin(), index * cols_);
    return MakeRange(start, cols_);
  }
  [[nodiscard]] constexpr auto Row(std::uint32_t index) const noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    auto start = std::next(data_.cbegin(), index * cols_);
    return MakeRange(start, cols_);
  }
  [[nodiscard]] constexpr void EraseRow(std::uint32_t index) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    auto range = Row(index);
    data_.erase(range.begin(), range.end());
    --rows_;
  }
  [[nodiscard]] constexpr void EraseCol(std::uint32_t index) noexcept {
    assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
    --cols_;
    auto iter = std::next(data_.begin(), index);
    for (std::uint32_t r = 0; r < rows_; r++) {
      iter = data_.erase(iter);
      if (r != rows_ - 1) {
        std::advance(iter, cols_);
      }
    }
  }
  [[nodiscard]] constexpr void AppendRow(ConstReference init_val) noexcept {
    data_.resize(data_.size() + cols_, init_val);
    ++rows_;
  }
  [[nodiscard]] constexpr void AppendCol(ConstReference init_val) noexcept {
    data_.resize(data_.size() + rows_);
    auto src = data_.begin() + (rows_ - 1) * cols_;
    auto dst = data_.end() - 1;
    // The first line does not move, so r starts from 1
    for (std::uint32_t r = 1; r < rows_; r++) {
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
  std::uint32_t rows_{0}, cols_{0};
  Container data_{};
};
}  // namespace XEngine