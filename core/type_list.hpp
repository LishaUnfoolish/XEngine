
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/12/01
@Desc: type list
***************************/

#pragma once
#include <type_traits>
namespace XEngine {
template <typename Type, typename>
using UnpackAsType = Type;

template <typename Type, typename = void>
struct SizeOf : std::integral_constant<std::size_t, 0u> {};

template <typename Type>
requires(requires { sizeof(Type); }) struct SizeOf<Type>
    : std::integral_constant<std::size_t, sizeof(Type)> {};

template <typename Type>
inline constexpr std::size_t size_of_v = SizeOf<Type>::value;

template <typename... Type>
struct TypeList {
  using type = TypeList;
  static constexpr auto size = sizeof...(Type);
};
template <typename... Type, typename... Other>
constexpr TypeList<Type..., Other...> operator+(TypeList<Type...>,
                                                TypeList<Other...>) {
  return {};
}
template <std::size_t, typename>
struct TypeListElement;
template <std::size_t Index, typename First, typename... Other>
struct TypeListElement<Index, TypeList<First, Other...>>
    : TypeListElement<Index - 1u, TypeList<Other...>> {};
template <typename First, typename... Other>
struct TypeListElement<0u, TypeList<First, Other...>> {
  using type = First;
};
template <std::size_t Index, typename List>
using TypeListElement_t = typename TypeListElement<Index, List>::type;

template <typename, typename>
struct TypeListIndex;

template <typename Type, typename First, typename... Other>
struct TypeListIndex<Type, TypeList<First, Other...>> {
  using value_type = std::size_t;
  static constexpr value_type value =
      1u + TypeListIndex<Type, TypeList<Other...>>::value;
};

template <typename Type, typename... Other>
requires(requires {
  TypeListIndex<Type, TypeList<Other...>>::value == sizeof...(Other);
}) struct TypeListIndex<Type, TypeList<Type, Other...>> {
  using value_type = std::size_t;
  static constexpr value_type value = 0u;
};

template <typename Type>
struct TypeListIndex<Type, TypeList<>> {
  using value_type = std::size_t;
  static constexpr value_type value = 0u;
};

template <typename Type, typename List>
inline constexpr std::size_t type_list_index_v =
    TypeListIndex<Type, List>::value;

template <typename...>
struct TypeListCat;
template <>
struct TypeListCat<> {
  using type = TypeList<>;
};
template <typename... Type, typename... Other, typename... List>
struct TypeListCat<TypeList<Type...>, TypeList<Other...>, List...> {
  using type = typename TypeListCat<TypeList<Type..., Other...>, List...>::type;
};
template <typename... Type>
requires(requires {
  typename TypeList<Type...>::type;
}) struct TypeListCat<TypeList<Type...>> {
  using type = TypeList<Type...>;
};
template <typename... List>
using TypeListCat_t = typename TypeListCat<List...>::type;

template <typename...>
struct type_list_unique;

template <>
struct type_list_unique<TypeList<>> {
  using type = TypeList<>;
};
template <typename Type, typename... Other>
requires(requires {
  typename TypeList<Type, Other...>::type;
}) struct type_list_unique<TypeList<Type, Other...>> {
  using type = std::conditional_t<
      (std::is_same_v<Type, Other> || ...),
      typename type_list_unique<TypeList<Other...>>::type,
      TypeListCat_t<TypeList<Type>,
                    typename type_list_unique<TypeList<Other...>>::type>>;
};
template <typename Type>
using TypeListUnique_t = typename type_list_unique<Type>::type;

template <typename List, typename Type>
struct TypeListContains;
template <typename... Type, typename Other>
requires(requires {
  typename TypeList<Type...>::type;
}) struct TypeListContains<TypeList<Type...>, Other>
    : std::disjunction<std::is_same<Type, Other>...> {};
template <typename List, typename Type>
inline constexpr bool TypeListContains_v = TypeListContains<List, Type>::value;

template <typename...>
struct TypeListDiff;
template <typename... Type, typename... Other>
requires(requires {
  typename TypeList<Type...>::type;
  typename TypeList<Other...>::type;
}) struct TypeListDiff<TypeList<Type...>, TypeList<Other...>> {
  using type = TypeListCat_t<
      std::conditional_t<TypeListContains_v<TypeList<Other...>, Type>,
                         TypeList<>, TypeList<Type>>...>;
};
template <typename... List>
using TypeListDiff_t = typename TypeListDiff<List...>::type;

template <typename, template <typename...> class>
struct TypeListTransform;

template <typename... Type, template <typename...> class Op>
requires(requires {
  typename TypeList<Type...>::type;
}) struct TypeListTransform<TypeList<Type...>, Op> {
  using type = TypeList<typename Op<Type>::type...>;
};

template <typename List, template <typename...> class Op>
using TypeListTransform_t = typename TypeListTransform<List, Op>::type;

template <std::size_t Index, auto Candidate>
class NTHArgument {
  template <typename Ret, typename... Args>
  static constexpr TypeList<Args...> PickUp(Ret (*)(Args...));

  template <typename Ret, typename Class, typename... Args>
  static constexpr TypeList<Args...> PickUp(Ret (Class ::*)(Args...));

  template <typename Ret, typename Class, typename... Args>
  static constexpr TypeList<Args...> PickUp(Ret (Class ::*)(Args...) const);

  template <typename Type, typename Class>
  static constexpr TypeList<Type> PickUp(Type Class ::*);

 public:
  using type = TypeListElement_t<Index, decltype(PickUp(Candidate))>;
};
template <std::size_t Index, auto Candidate>
template <typename Ret, typename... Args>
inline constexpr TypeList<Args...> NTHArgument<Index, Candidate>::PickUp(
    Ret (*)(Args...)) {
  return TypeList<Args...>();
}
template <std::size_t Index, auto Candidate>
template <typename Type, typename Class>
inline constexpr TypeList<Type> NTHArgument<Index, Candidate>::PickUp(
    Type Class::*) {
  return TypeList<Type>();
}
template <std::size_t Index, auto Candidate>
using NTHArgument_t = typename NTHArgument<Index, Candidate>::type;

}  // namespace XEngine
