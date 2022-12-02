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

#include "traits/hash_traits.hpp"
#include "traits/type_name.hpp"
namespace XEngine {
using namespace std::literals;
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

struct TypeInfo final {
  template <typename Type>
  constexpr TypeInfo(std::in_place_type_t<Type>) noexcept
      : seq{TypeIndex<
            std::remove_cv_t<std::remove_reference_t<Type>>>::value()},
        identifier{
            TypeHash<std::remove_cv_t<std::remove_reference_t<Type>>>::value()},
        alias{TypeName<
            std::remove_cv_t<std::remove_reference_t<Type>>>::value()} {}

  [[nodiscard]] constexpr IdType index() const noexcept { return seq; }
  [[nodiscard]] constexpr IdType hash() const noexcept { return identifier; }
  [[nodiscard]] constexpr std::string_view name() const noexcept {
    return alias;
  }

 private:
  IdType seq;
  IdType identifier;
  std::string_view alias;
};

[[nodiscard]] inline constexpr bool operator==(const TypeInfo &lhs,
                                               const TypeInfo &rhs) noexcept {
  return lhs.hash() == rhs.hash();
}
[[nodiscard]] inline constexpr bool operator!=(const TypeInfo &lhs,
                                               const TypeInfo &rhs) noexcept {
  return !(lhs == rhs);
}
[[nodiscard]] constexpr bool operator<(const TypeInfo &lhs,
                                       const TypeInfo &rhs) noexcept {
  return lhs.index() < rhs.index();
}
[[nodiscard]] constexpr bool operator<=(const TypeInfo &lhs,
                                        const TypeInfo &rhs) noexcept {
  return !(rhs < lhs);
}
[[nodiscard]] constexpr bool operator>(const TypeInfo &lhs,
                                       const TypeInfo &rhs) noexcept {
  return rhs < lhs;
}
[[nodiscard]] constexpr bool operator>=(const TypeInfo &lhs,
                                        const TypeInfo &rhs) noexcept {
  return !(lhs < rhs);
}

template <typename Type>
[[nodiscard]] const TypeInfo &TypeId() noexcept {
  if constexpr (std::is_same_v<
                    Type, std::remove_cv_t<std::remove_reference_t<Type>>>) {
    static TypeInfo instance{std::in_place_type<Type>};
    return instance;
  } else {
    return TypeId<std::remove_cv_t<std::remove_reference_t<Type>>>();
  }
}

template <typename Type>
[[nodiscard]] const TypeInfo &TypeId(Type &&) noexcept {
  return TypeId<std::remove_cv_t<std::remove_reference_t<Type>>>();
}

}  // namespace XEngine
