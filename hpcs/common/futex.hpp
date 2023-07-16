#pragma once

#include <linux/futex.h>
#include <sys/syscall.h>

#include <climits>
namespace XEngine {
inline int Futex(int* data, int operation, int value,
                 const struct timespec* timeout, int* data2, int value2) {
  return syscall(SYS_futex, data, operation, value, timeout, data2, value2);
}

inline int FutexWait(void* futex, int comparand) {
  return XEngine::Futex(reinterpret_cast<int*>(futex), FUTEX_WAIT_PRIVATE,
                        comparand, nullptr, nullptr, 0);
}

inline int FutexWakeupOne(void* futex) {
  return XEngine::Futex(reinterpret_cast<int*>(futex), FUTEX_WAKE_PRIVATE, 1,
                        nullptr, nullptr, 0);
}

inline int FutexWakeupAll(void* futex) {
  return XEngine::Futex(reinterpret_cast<int*>(futex), FUTEX_WAKE_PRIVATE,
                        INT_MAX, nullptr, nullptr, 0);
}

}  // namespace XEngine