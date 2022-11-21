/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/22
@Desc: 静态类型标识
***************************/
#pragma once
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <utility>

namespace XEngine {
using TypeIndex = std::type_index;
using IdType = std::hash<std::type_index>::result_type;

template <typename Type, typename = void>
struct TypeHash final {
  [[nodiscard]] static constexpr IdType value() noexcept {
    return std::hash<std::type_index>{}(typeid(Type));
  }
  [[nodiscard]] constexpr operator IdType() const noexcept { return value(); }
};

template <auto Value>
using integral_constant = std::integral_constant<decltype(Value), Value>;
template <IdType Value>
using tag = integral_constant<Value>;

}  // namespace XEngine
