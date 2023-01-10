/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/01/10
@Desc: macros
***************************/
#pragma once
#include <concepts>
namespace XEngine {
#define HAS_MEMBER_TRAITS(name)                      \
  template <typename T, typename... Args>            \
  static constexpr bool HasMember##name = requires { \
    {T{}.name(std::forward<Args>()...)};             \
  };

#define HAS_STATIC_MEMBER_TRAITS(name)                     \
  template <typename T, typename... Args>                  \
  static constexpr bool HasStaticMember##name = requires { \
    {T::name(std::declval<Args>()...)};                    \
  };

#define HAS_OPERATOR_TRAITS(op, name)                  \
  template <typename T1, typename T2>                  \
  static constexpr bool HasOperator##name = requires { \
    {T1{} op T2{}};                                    \
  };
}  // namespace XEngine