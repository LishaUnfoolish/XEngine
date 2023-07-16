/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/18
***************************/
#pragma once

#include <atomic>
#include <cassert>
#include <coroutine>
#include <cstdint>
#include <functional>
#include <string>
#include <utility>
namespace XEngine {
enum class CoroutineState { READY, DATA_WAIT, FINISHED };

template <typename T>
class Future {
 public:
  struct promise_type {
    Future get_return_object() { return Future{Handle::from_promise(*this)}; }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept {
      value_ = CoroutineState::FINISHED;
      return {};
    }
    std::suspend_always yield_value(T& ref) noexcept {
      value_ = ref;
      return {};
    }
    std::suspend_always yield_value(T&& ref) noexcept {
      value_ = ref;
      return {};
    }
    void return_value(T v) { value_ = v; }
    void unhandled_exception() {
      assert(false && "Future unhandled_exception");
    }

    T get_value() { return value_; }
    bool set_value(T value) {
      value_ = value;
      return true;
    }

   private:
    T value_{};
  };

  using Handle = std::coroutine_handle<promise_type>;
  Future() = default;
  Future(const Future&) = delete;
  Future& operator=(const Future&) = delete;
  explicit Future(Handle h) : handle_(h) {}
  Future(Future&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
  Future& operator=(Future&& other) noexcept {
    if (this != &other) {
      if (handle_.address() != nullptr && handle_) { handle_.destroy(); }
      handle_ = std::exchange(other.handle_, {});
    }
    return *this;
  }
  ~Future() {
    if (handle_.address() != nullptr && handle_) {
      Stop();
      handle_.destroy();
    }
  }
  bool Resume() {
    if (handle_.address() == nullptr) { return false; }
    handle_.resume();
    return true;
  }
  void Stop() {
    if (handle_.address() == nullptr) { return; }
    handle_.promise().set_value(CoroutineState::FINISHED);
  }
  T get_value() {
    if (handle_.address() == nullptr) { return CoroutineState::FINISHED; }
    return handle_.promise().get_value();
  }
  bool set_value(T state) {
    if (handle_.address() == nullptr) { return false; }
    return handle_.promise().set_value(state);
  }
  [[nodiscard]] bool Done() const {
    if (handle_.address() == nullptr) { return true; }
    return handle_.done();
  }

 private:
  Handle handle_{};
};

///////////////
using CoroutineFunc = std::function<XEngine::Future<XEngine::CoroutineState>()>;
class Coroutine {
 public:
  Coroutine() = default;
  explicit Coroutine(const CoroutineFunc& fn)
      : fun_(std::move(fn)), future_(fun_()) {}
  virtual ~Coroutine() {
    Acquire();
    Stop();
    Release();
  }

  void SetFunction(const CoroutineFunc& fn) {
    fun_ = fn;
    future_ = fun_();
  }

  [[nodiscard]] uint64_t Id() const { return id_; }
  void SetId(uint64_t id) { id_ = id; }
  [[nodiscard]] const std::string& Name() const { return name_; }
  void SetName(const std::string& name) { name_ = name; }
  void SetGroupName(const std::string& GroupName) { group_name_ = GroupName; }
  const std::string& GroupName() { return group_name_; }
  [[nodiscard]] std::uint32_t Priority() const { return priority_; }
  void SetPriority(uint32_t priority) { priority_ = priority; }

  void Resume() { future_.Resume(); }
  bool Acquire() { return !lock_.test_and_set(std::memory_order_acquire); }
  void Release() { return lock_.clear(std::memory_order_release); }
  CoroutineState State() { return future_.get_value(); }
  bool SetState(CoroutineState state) { return future_.set_value(state); }
  void Stop() { future_.Stop(); }
  bool Done() { return future_.Done(); }

 private:
  std::string name_{};
  uint64_t id_ = 0;
  CoroutineFunc fun_;
  uint32_t priority_ = 0;
  std::string group_name_ = "default_group_name";
  Future<CoroutineState> future_;
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
};

}  // namespace XEngine