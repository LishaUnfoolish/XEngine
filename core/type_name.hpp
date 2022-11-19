
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/20
@Desc: type name模板接口
***************************/

#pragma once
#include <string_view>
#include <type_traits>

#include "core/type_info.hpp"
namespace XEngine {
namespace internal {
#define USED_FULL_TYPE_NAME false  //使用详细type name,is not a literal type.
#if USED_FULL_TYPE_NAME
#include <cxxabi.h>
template <typename T>
struct type_name final {
  const char *name;
  type_name() : name(abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr)) {}
  ~type_name() { std::free((void *)name); }
  constexpr operator const char *() { return name; }
};

#else
template <typename Type>
[[nodiscard]] constexpr auto type_name() noexcept {
  std::string_view pretty_function{__PRETTY_FUNCTION__};
  auto first = pretty_function.find_first_not_of(
      ' ', pretty_function.find_first_of('=') + 1);
  auto value =
      pretty_function.substr(first, pretty_function.find_last_of(']') - first);
  return value;
}

template <typename Type, auto = type_name<Type>().find_first_of('.')>
[[nodiscard]] static constexpr std::string_view type_name(int) noexcept {
  constexpr auto value = type_name<Type>();
  return value;
}
#endif

}  // namespace internal
}  // namespace XEngine
