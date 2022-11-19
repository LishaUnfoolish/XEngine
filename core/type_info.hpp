/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/20
@Desc: 静态类型标识
***************************/
#pragma once
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <utility>

namespace XEngine {
using type_index = std::type_index;
using id_type = std::hash<std::type_index>::result_type;

template <typename Type, typename = void>
struct type_hash final {
  [[nodiscard]] static constexpr id_type value() noexcept {
    return std::hash<std::type_index>{}(typeid(Type));
  }
  [[nodiscard]] constexpr operator id_type() const noexcept { return value(); }
};

}  // namespace XEngine
