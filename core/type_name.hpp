
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/23
@Desc: type name模板接口
***************************/

#pragma once
#include <string_view>
#include <type_traits>

namespace XEngine {
namespace internal {
#define USED_FULL_TYPE_NAME false  //使用详细type name,is not a literal type.
#if USED_FULL_TYPE_NAME
#include <cxxabi.h>
template <typename T>
struct TypeName final {
  const char *name;
  TypeName() : name(abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr)) {}
  ~TypeName() { std::free((void *)name); }
  constexpr operator const char *() { return name; }
};

#else
template <typename Type>
[[nodiscard]] constexpr auto TypeName() noexcept {
  std::string_view pretty_function{__PRETTY_FUNCTION__};
  auto first = pretty_function.find_first_not_of(
      ' ', pretty_function.find_first_of('=') + 1);
  auto value =
      pretty_function.substr(first, pretty_function.find_last_of(']') - first);
  return value;
}

template <typename Type, auto = TypeName<Type>().find_first_of('.')>
[[nodiscard]] static constexpr std::string_view TypeName(int) noexcept {
  constexpr auto value = TypeName<Type>();
  return value;
}
#endif

}  // namespace internal
template <typename Type, typename = void>
struct TypeName final {
  [[nodiscard]] static constexpr std::string_view value() noexcept {
    return internal::TypeName<Type>(0);
  }

  [[nodiscard]] constexpr operator std::string_view() const noexcept {
    return value();
  }
};

}  // namespace XEngine
