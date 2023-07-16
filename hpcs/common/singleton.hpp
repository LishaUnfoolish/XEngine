/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
#pragma once
#include "hpcs/common/macros.hpp"

HAS_MEMBER_TRAITS(Shutdown);

template <typename T>
class Singleton {
 public:
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  static T* Instance(bool create = true) {
    static std::atomic<T*> instance{nullptr};
    if (!instance && create) {
      static std::once_flag flag;
      std::call_once(flag, [&] { instance = new (std::nothrow) T(); });
    }
    return instance;
  }
  static void Destroy() {
    if constexpr (HasMemberShutdown<T>) {
      auto instance = Instance(false);
      if (instance != nullptr) { instance->Shutdown(); }
    }
  }

 private:
  Singleton() = default;
  friend T;
};

#define DECLARE_SINGLETON(classname)                         \
 public:                                                     \
  static classname* Instance(bool create = true) {           \
    return Singleton<classname>::Instance();                 \
  }                                                          \
                                                             \
  static void CleanUp() { Singleton<classname>::Destroy(); } \
                                                             \
  classname(const classname&) = delete;                      \
  classname& operator=(const classname&) = delete;