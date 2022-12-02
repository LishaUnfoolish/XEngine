/***********************************************************************************************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/23
@Desc: Fowler-Noll-Vo-1a Compile-time hash_
************************************************************************************************************/
#pragma once
#include <typeindex>
namespace XEngine {
using IdType = std::hash<std::type_index>::result_type;

/*
prime_:
  32 bit  = 2^24 + 2^8 + 0x93;
  64 bit  = 2^40 + 2^8 + 0xb3;
  128 bit  = 2^88 + 2^8 + 0x3b;
  256 bit  = 2^168 + 2^8 + 0x63;
  512 bit  = 2^344 + 2^8 + 0x57;
  1024 bit = 2^680 + 2^8 + 0x8d;

offset_:
  32 bit offset_basis = 2166136261;
  64 bit offset_basis = 14695981039346656037;
  128 bit offset_basis = 144066263297769815596495629667062367629;
  256 bit offset_basis =
    100029257958052580907070968620625704837092796014241193945225284501741471925557;
  512 bit offset_basis =
    9659303129496669498009435400716310466090418745672637896108374329434462657994582932197716438449813051892206539805784495328239340083876191928701583869517785;
  1024 bit offset_basis =
    1419779506494762106872207064140321832088062279544193396087847491461758272325229673230371772215086409652120235554936562817466910857181476047101507614;
*/

/***********************************************************************************************************
算法描述：
  相关变量：
   * hash值：一个n位的unsigned int型hash值
   * offset_basis：初始的哈希值
   * FNV_prime：FNV用于散列的质数
   * octet_of_data：8位数据（即一个字节）
   * FNV-1描述：
      * hash = offset_basis
      * for each octet_of_data to be hashed
          * hash = hash * FNV_prime
          * hash = hash xor octet_of_data
      * return hash
    * FNV-1a描述：
      * hash = offset_basis
      * for each octet_of_data to be hashed
          * hash = hash xor octet_of_data
          * hash = hash * FNV_prime
      * return hash
************************************************************************************************************/
template <typename>
struct HashParameters;
template <>
struct HashParameters<std::uint32_t> {
  using type = std::uint32_t;
  static constexpr std::uint32_t offset_{2166136261};
  static constexpr std::uint32_t prime_{2 ^ 24 + 2 ^ 8 + 0x93};
  ;
};
template <>
struct HashParameters<std::uint64_t> {
  using type = std::uint64_t;
  static constexpr std::uint64_t offset_{14695981039346656037ull};
  static constexpr std::uint64_t prime_{2 ^ 40 + 2 ^ 8 + 0xb3};
};

template <typename DATA>
struct HashedTraits {
  const DATA *data_;
  XEngine::IdType hash_;
};

template <typename DATA>
class HashString : HashedTraits<DATA> {
 public:
  constexpr HashString(const DATA *data) noexcept
      : HashedTraits<DATA>{Helper(data)} {}
  [[nodiscard]] constexpr operator XEngine::IdType() const noexcept {
    return HashedTraits<DATA>::hash_;
  }
  [[nodiscard]] constexpr operator const DATA *() const noexcept {
    return HashedTraits<DATA>::data_;
  }

  static constexpr auto Helper = [](const DATA *str) -> HashedTraits<DATA> {
    XEngine::IdType length{0};
    HashedTraits<DATA> data{str, HashParameters<XEngine::IdType>::offset_};
    for (; str[length]; ++length) {
      data.hash_ ^=
          static_cast<HashParameters<XEngine::IdType>::type>(str[length]);
      data.hash_ *= HashParameters<XEngine::IdType>::prime_;
    }
    return data;
  };
};
template <typename Char>
[[nodiscard]] constexpr bool operator==(const HashString<Char> &lhs,
                                        const HashString<Char> &rhs) noexcept {
  return lhs.operator XEngine::IdType() == rhs.operator XEngine::IdType();
}
template <typename Type, typename = void>
struct TypeIndex final {
  [[nodiscard]] static IdType value() noexcept {
    static const IdType value = std::type_index(typeid(Type)).hash_code();
    return value;
  }
  [[nodiscard]] constexpr operator IdType() const noexcept { return value(); }
};

}  // namespace XEngine

namespace std::literals {
[[nodiscard]] constexpr XEngine::HashString<char> operator"" _hash(
    const char *str, const std::size_t) noexcept {
  return XEngine::HashString<char>{str};
}

}  // namespace std::literals
