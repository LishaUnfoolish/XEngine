// necessary header for Expression Template library
// https://codesire-deng.github.io/2022/01/31/Expression-Templates/
#include <functional>
#include <type_traits>

// header for user-define things
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

// helper class for CRTP. For more info, please see my previous blog
template <class T, template <typename> class Interface>
struct CRTP {
  T& self() { return static_cast<T&>(*this); }
  const T& self() const { return static_cast<const T&>(*this); }

 private:
  CRTP() = default;
  friend Interface<T>;
};

// my Expression Template library implementation
namespace ET {

// base class for lazy evaluation
template <class T>
struct Expression : CRTP<T, Expression> {
  // use a looong function name to avoid ambiguity
  constexpr auto evaluate(size_t i) const { return this->self()[i]; }
  constexpr auto selfSize() const { return this->self().size(); }
};

// class to indicate a single value (instead of an vector)
template <class T>
struct ArithmeticExpression : Expression<ArithmeticExpression<T>> {
  ArithmeticExpression() = delete;

  template <std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  constexpr ArithmeticExpression(const T value) noexcept : val(value) {}

  constexpr T operator[](size_t) const { return val; }
  constexpr void size() const {}  // return void for easier metaprogramming

 private:
  const T val;
};

// class to indicate an vector (instead of a single arithmetic value)
template <class T>
struct VectorExpression : Expression<T> {};

template <typename BinaryOperator, typename LHS, typename RHS>
struct BinaryExpression
    : VectorExpression<BinaryExpression<BinaryOperator, LHS, RHS>> {
  constexpr auto operator[](size_t i) const {
    // evaluation happens when operator[] is called
    return BinaryOperator{}(lhs.evaluate(i), rhs.evaluate(i));
  }
  constexpr size_t size() const {
    // return the size of an *vector*
    if constexpr (!std::is_same_v<decltype(lhs.selfSize()), void>)
      return lhs.selfSize();
    else
      return rhs.selfSize();
  }

  BinaryExpression(const LHS& lhs, const RHS& rhs) : lhs(lhs), rhs(rhs) {}

 private:
  const LHS& lhs;
  const RHS& rhs;
};

template <typename LHS, typename RHS>
using AddExpression = BinaryExpression<std::plus<>, LHS, RHS>;

template <typename LHS, typename RHS>
using SubExpression = BinaryExpression<std::minus<>, LHS, RHS>;

template <typename LHS, typename RHS>
using MulExpression = BinaryExpression<std::multiplies<>, LHS, RHS>;

template <typename LHS, typename RHS>
using DivExpression = BinaryExpression<std::divides<>, LHS, RHS>;

}  // namespace ET

// ==============================

// overloaded operator to export:

template <class T>
using ETNum = ET::ArithmeticExpression<T>;

template <typename E1, typename E2>
constexpr auto operator+(const ET::Expression<E1>& lhs,
                         const ET::Expression<E2>& rhs) {
  // notice Expression<E1> -> E1
  // so "Expression" is removed
  return ET::AddExpression<E1, E2>(lhs.self(), rhs.self());
}

template <typename E1, typename E2>
constexpr auto operator-(const ET::Expression<E1>& lhs,
                         const ET::Expression<E2>& rhs) {
  return ET::SubExpression<E1, E2>(lhs.self(), rhs.self());
}

template <typename E1, typename E2>
constexpr auto operator*(const ET::Expression<E1>& lhs,
                         const ET::Expression<E2>& rhs) {
  return ET::MulExpression<E1, E2>(lhs.self(), rhs.self());
}

template <typename E1, typename E2>
constexpr auto operator/(const ET::Expression<E1>& lhs,
                         const ET::Expression<E2>& rhs) {
  return ET::DivExpression<E1, E2>(lhs.self(), rhs.self());
}

// ==============================

// user-define operator

// I find it hard to convert a function into an operator() overloading.
// If you have a good idea, please tell me!
struct POW {
  template <typename _Tp, typename _Up>
  auto operator()(_Tp&& __t, _Up&& __u) const {
    return pow(std::forward<_Tp>(__t), std::forward<_Up>(__u));
  }
};

template <typename E1, typename E2>
constexpr auto Pow(const ET::Expression<E1>& lhs,
                   const ET::Expression<E2>& rhs) {
  return ET::BinaryExpression<POW, E1, E2>(lhs.self(), rhs.self());
}

// ==============================

// user-define vector

template <typename T, class Container = std::vector<T>>
struct Vec : Container, ET::VectorExpression<Vec<T, Container>> {
  using Container::Container;

  template <typename Exp>
  Vec& operator=(const ET::Expression<Exp>& exp) {
    const size_t n = exp.selfSize();
    this->resize(n);
    for (size_t i = 0; i < n; ++i) this->operator[](i) = exp.evaluate(i);
    return *this;
  }
};

// ==============================

int main() {
  Vec<double> ans, a(5, 1), b(5, 2), c(5, 8), d(5, 7), e(5, 0.4);

  ans = Pow((a + b) * c / d + ETNum<double>(1), e);

  for (auto&& x : ans) {
    std::cout << x << ' ';
    assert(x == pow(((1 + 2) * 8 / 7.0 + 1), 0.4));
  }

  return 0;
}
