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
// template <typename T>
// class Matrix {
//  public:
//   using Container = std::vector<T>;
//   using ConstReference = typename Container::const_reference;
//   Matrix() = default;
//   constexpr Matrix(
//       const NodeId& rows, const NodeId& cols,
//       ConstReference
//           init_val) noexcept(std::is_nothrow_default_constructible_v<T>)
//       : rows_(rows), cols_(cols) {
//     data_.assign(rows * cols, init_val);
//   }

//   constexpr void Resize(const NodeId& rows, const NodeId& cols,
//                                       const ConstReference& init_val)
//                                       noexcept {
//     rows_ = rows;
//     cols_ = cols;
//     data_.assign(rows * cols, init_val);
//   }

//   constexpr void Reserve(const NodeId& rows,
//                                        const NodeId& cols) noexcept {
//     data_.reserve(rows * cols);
//   }
//   [[nodiscard]] constexpr NodeId Rows() const noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     return rows_;
//   }
//   [[nodiscard]] constexpr NodeId Cols() const noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     return cols_;
//   }
//   constexpr void Insert(const NodeId& row, const NodeId& col,
//                                       const ConstReference& value) noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     data_[row * cols_ + col] = value;
//   }
//   // Returns the range of iterations for the index row
//   [[nodiscard]] constexpr auto Row(const NodeId& index) noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     auto start = std::next(data_.begin(), index * cols_);
//     return MakeRange(start, cols_);
//   }
//   [[nodiscard]] constexpr auto Row(const NodeId& index) const noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     auto start = std::next(data_.cbegin(), index * cols_);
//     return MakeRange(start, cols_);
//   }
//   constexpr void EraseRow(const NodeId& index) noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     auto range = Row(index);
//     data_.erase(range.begin(), range.end());
//     --rows_;
//   }
//   constexpr void EraseCol(const NodeId& index) noexcept {
//     assert(rows_ != 0 && cols_ != 0 && !data_.empty() && "Invalid matrix");
//     --cols_;
//     auto iter = std::next(data_.begin(), index);
//     for (NodeId r = 0; r < rows_; r++) {
//       iter = data_.erase(iter);
//       if (r != rows_ - 1) { std::advance(iter, cols_); }
//     }
//   }
//   constexpr void AppendRow(
//       const ConstReference& init_val) noexcept {
//     data_.resize(data_.size() + cols_, init_val);
//     ++rows_;
//   }
//   constexpr void AppendCol(
//       const ConstReference& init_val) noexcept {
//     data_.resize(data_.size() + rows_);
//     auto src = data_.begin() + (rows_ - 1) * cols_;
//     auto dst = data_.end() - 1;
//     // The first line does not move, so r starts from 1
//     for (NodeId r = 1; r < rows_; r++) {
//       std::copy_backward(src, src + cols_, dst);
//       *dst = init_val;
//       src -= cols_;
//       dst -= cols_ + 1;
//     }
//     // Assign a value to the new column in the first row
//     *(src + cols_) = init_val;
//     ++cols_;
//   }

//  private:
//   NodeId rows_{0};
//   NodeId cols_{0};
//   Container data_{};
// };

template <typename T>
class Matrix {
 public:
  using Container = std::vector<T>;
  using ConstReference = typename Container::const_reference;
  // 构造函数
  Matrix() = default;

  Matrix(NodeId rows, NodeId cols) : m_rows_(rows), m_cols_(cols) {
    m_data.resize(rows, std::vector<T>(cols, T{}));
  }
  constexpr Matrix(const NodeId& rows, const NodeId& cols,
                   const NodeId init_val) noexcept
      : m_rows_(rows), m_cols_(cols) {
    m_data.resize(rows, std::vector<T>(cols, init_val));
  }
  // 复制构造函数
  Matrix(const Matrix& other)
      : m_rows_(other.m_rows_), m_cols_(other.m_cols_), m_data(other.m_data) {}

  // 赋值运算符
  Matrix& operator=(const Matrix& other) {
    m_rows_ = other.m_rows_;
    m_cols_ = other.m_cols_;
    m_data = other.m_data;
    return *this;
  }

  // 索引运算符
  std::vector<T>& operator[](NodeId index) { return m_data[index]; }

  const std::vector<T>& operator[](NodeId index) const { return m_data[index]; }

  // 获取行数和列数
  NodeId Rows() const { return m_rows_; }
  NodeId Cols() const { return m_cols_; }

  // void AppendCol(const T& init_val) {
  //   for (auto& row : m_data) { row.push_back(init_val); }
  //   ++m_cols_;
  // }
  void AppendCol(const T& init_val) {
#pragma omp parallel for shared(m_data)
    for (NodeId i = 0; i < m_data.size(); ++i) {
      m_data[i].push_back(init_val);
    }
#pragma omp atomic
    ++m_cols_;
  }
  void AppendRow(const T& init_val) {
    m_data.push_back(std::vector<T>(m_cols_, init_val));
    ++m_rows_;
  }

  void Insert(const NodeId& row, const NodeId& col, const T& value) noexcept {
    m_data[row][col] = value;
  }

  auto Row(const NodeId& index) noexcept {
    return MakeRange(m_data[index].begin(), m_data[index].end());
  }
  auto Row(const NodeId& index) const noexcept {
    return MakeRange(m_data[index].begin(), m_data[index].end());
  }

  auto Col(const NodeId& index) noexcept {
    std::vector<T> col;
    col.reserve(m_rows_);
#pragma omp parallel for reduction(+ : col)
    for (std::size_t i = 0; i < m_rows_; i++) {
      col.push_back(m_data[i][index]);
    }
    return col;
  }

  auto Col(const NodeId& index) const noexcept {
    std::vector<T> col;
    col.reserve(m_rows_);
#pragma omp parallel for reduction(+ : col)
    for (std::size_t i = 0; i < m_rows_; i++) {
      col.push_back(m_data[i][index]);
    }
    return col;
  }

  void EraseRow(const NodeId& index) noexcept {
    m_data.erase(m_data.begin() + index);
    --m_rows_;
  }
  // void EraseCol(const NodeId& index) noexcept {
  //   for (auto& row : m_data) { row.erase(row.begin() + index); }
  //   --m_cols_;
  // }
  void EraseCol(const NodeId& index) noexcept {
#pragma omp parallel for shared(m_data)
    for (auto it = m_data.begin(); it < m_data.end(); ++it) {
      it->erase(it->begin() + index);
    }
    --m_cols_;
  }

  // void Reserve(const NodeId& rows, const NodeId& cols) {
  //   m_data.reserve(rows);
  //   for (auto& row : m_data) { row.reserve(cols); }
  // }
  void Reserve(const NodeId& rows, const NodeId& cols) {
    m_data.reserve(rows);
#pragma omp parallel for shared(m_data)
    for (int i = 0; i < m_data.size(); ++i) { m_data[i].reserve(cols); }
  }

 private:
  NodeId m_rows_{};
  NodeId m_cols_{};
  std::vector<Container> m_data;
};

}  // namespace XEngine