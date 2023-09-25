/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/01/10
@Desc: macros
***************************/
#pragma once
#include <concepts>
#include <cstdint>
#include <limits>
#include <new>
#include <type_traits>

namespace XEngine {
#define HAS_MEMBER_TRAITS(name)                                              \
  template <typename T, typename... Args>                                    \
  [[maybe_unused]] static constexpr bool HasMember##name = requires(T&& t) { \
    { t.name(std::forward<Args>()...) };                                     \
  };

#define HAS_MEMBER_RET_TRAITS(name)                                            \
  template <typename T, typename Ret, typename... Args>                        \
  [[maybe_unused]] static constexpr bool HasRetMember##name = requires(T&& t) {                 \
    { t.name(std::forward<Args>()...) };                                       \
    requires std::is_same_v<Ret, decltype(T{}.name(std::forward<Args>()...))>; \
  };

#define HAS_STATIC_MEMBER_TRAITS(name)                     \
  template <typename T, typename... Args>                  \
  [[maybe_unused]] static constexpr bool HasStaticMember##name = requires { \
    { T::name(std::declval<Args>()...) };                  \
  };

#define HAS_OPERATOR_TRAITS(op, name)                         \
  template <typename T, typename... Args>                     \
  [[maybe_unused]] static constexpr bool HasOperator##name = requires(T&& t) { \
    { t.operator op(std::forward<Args>()...) };               \
  };

using NodeId = std::uint32_t;
[[maybe_unused]] inline static constexpr NodeId NodeIdNone = std::numeric_limits<NodeId>::max();

// https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │
// ...
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif
}  // namespace XEngine