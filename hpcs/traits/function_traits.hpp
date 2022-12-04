/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/12/05
@Desc: function模板接口
***************************/
#pragma once
#include <iterator>
#include <type_traits>
namespace XEngine {
template <typename, typename>
struct IsApplicable : std::false_type {};
template <typename Func, template <typename...> class Tuple, typename... Args>
struct IsApplicable<Func, Tuple<Args...>> : std::is_invocable<Func, Args...> {};
template <typename Func, template <typename...> class Tuple, typename... Args>
struct IsApplicable<Func, const Tuple<Args...>>
    : std::is_invocable<Func, Args...> {};
template <typename Func, typename Args>
inline constexpr bool IsApplicable_v = requires {
  requires IsApplicable<Func, Args>::value;
};

template <typename, typename, typename>
struct IsApplicableReturn : std::false_type {};
template <typename Ret, typename Func, typename... Args>
struct IsApplicableReturn<Ret, Func, std::tuple<Args...>>
    : std::is_invocable_r<Ret, Func, Args...> {};
template <typename Ret, typename Func, typename Args>
inline constexpr bool IsApplicableReturn_v = requires {
  requires IsApplicableReturn<Ret, Func, Args>::value;
};
template <typename Type>
inline constexpr bool IsComplete_v = requires {
  {sizeof(Type)};
};

template <typename Type>
inline constexpr bool IsIterator_v = requires {
  requires !std::is_same_v<std::remove_cv_t<std::remove_pointer_t<Type>>, void>;
  typename std::iterator_traits<Type>::iterator_category;
};

}  // namespace XEngine