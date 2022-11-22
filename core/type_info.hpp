/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/23
@Desc: 静态类型标识
***************************/
#pragma once
#include <string_view>
#include <type_traits>
#include <utility>

#include "core/hash_traits.hpp"
#include "core/type_name.hpp"
namespace XEngine {

template <typename Type, typename = void>
struct TypeHash final {
  [[nodiscard]] static constexpr IdType value() noexcept {
    return HashString(internal::TypeName<Type>().data());
  }
  [[nodiscard]] constexpr operator IdType() const noexcept { return value(); }
};

template <auto Value>
using IntegralConstant = std::integral_constant<decltype(Value), Value>;

template <IdType Value>
using tag = IntegralConstant<Value>;

template <typename To, typename From>
struct ConstnessAs {
  using type = std::remove_const_t<To>;
};
template <typename To, typename From>
struct ConstnessAs<To, const From> {
  using type = const To;
};
template <typename To, typename From>
using ConstnessAs_t = typename ConstnessAs<To, From>::type;

template <typename Member>
class MemberClass {
  static_assert(std::is_member_pointer_v<Member>,
                "Invalid pointer type to non-static member object or function");
  template <typename Class, typename Ret, typename... Args>
  static Class *C(Ret (Class::*)(Args...));
  template <typename Class, typename Ret, typename... Args>
  static Class *C(Ret (Class::*)(Args...) const);
  template <typename Class, typename Type>
  static Class *C(Type Class::*);

 public:
  using type = std::remove_pointer_t<decltype(C(std::declval<Member>()))>;
};
template <typename Member>
using MemberClass_t = typename MemberClass<Member>::type;

}  // namespace XEngine
