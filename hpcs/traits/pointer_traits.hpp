/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
namespace XEngine {
template <typename T>
struct IsSmartPointerHelper : public std::false_type {};
template <typename T>
struct IsSmartPointerHelper<std::shared_ptr<T>> : public std::true_type {
  using valueType = T;
};
template <typename T>
struct IsSmartPointerHelper<std::unique_ptr<T>> : public std::true_type {
  using valueType = T;
};
template <typename T>
struct IsSmartPointerHelper<std::weak_ptr<T>> : public std::true_type {
  using valueType = T;
};
template <typename T>
struct IsSmartPointer
    : public IsSmartPointerHelper<typename std::remove_cv<T>::type> {};

template <typename T>
inline constexpr bool CheckSmartPointer = requires {
  requires IsSmartPointer<T>::value;
};

}  // namespace XEngine