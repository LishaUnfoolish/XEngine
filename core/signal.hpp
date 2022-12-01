/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/22
@Desc: 信号与槽接口
***************************/

#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <shared_mutex>
#include <utility>

namespace XEngine {

template <typename... Args>
class Connection;
template <typename T>
class Signal;
template <typename T>
class Slot;
template <typename Key, typename Value>
class Dispatcher;

template <typename... Args>
class SlotBase {
 public:
  using Callback = std::function<void(Args...)>;
  SlotBase(const SlotBase& another)
      : cb_(another.cb_), connected_(another.connected_) {}
  explicit SlotBase(const Callback& cb, bool connected = true)
      : cb_(cb), connected_(connected) {}
  virtual ~SlotBase() {}

  void operator()(Args... args) {
    if (connected_ && cb_) {
      cb_(std::forward<Args>(args)...);
    }
  }

  void Disconnect() { connected_ = false; }
  bool connected() const { return connected_; }

 private:
  Callback cb_;
  bool connected_ = true;
};

template <typename RT, typename... Args>
class Slot<RT(Args...)> : public SlotBase<Args...> {
  using Callback = std::function<RT(Args...)>;

 public:
  explicit Slot(const Callback& call_back, bool connected = true)
      : SlotBase<Args...>(nullptr, connected), call_back_(call_back) {}
  virtual ~Slot() {}

  RT operator()(Args... args) {
    return CallBack<RT>(std::forward<Args>(args)...);
  }

 private:
  template <typename Rt>
  auto CallBack(Args... args) const -> Rt requires(!std::is_void<RT>::value) {
    if (SlotBase<Args...>::connected() && call_back_) {
      return call_back_(std::forward<Args>(args)...);
    }
    return Rt{};
  }

  template <typename Rt>
  auto CallBack(Args... args) const -> Rt requires(std::is_void<Rt>::value) {
    if (SlotBase<Args...>::connected() && call_back_) {
      return call_back_(std::forward<Args>(args)...);
    }
  }

  Callback call_back_;
};

template <typename RT, typename... Args>
class Signal<RT(Args...)> {
  using Callback = std::function<RT(Args...)>;
  using SlotPtr = std::shared_ptr<Slot<RT(Args...)>>;
  using SlotList = std::list<SlotPtr>;
  using ConnectionType = Connection<RT(Args...)>;

 public:
  explicit Signal(const Callback& call_back) { Connect(call_back); }
  Signal() {}
  virtual ~Signal() { DisconnectAllSlots(); }

  RT operator()(Args... args) {
    return CallBack<RT>(std::forward<Args>(args)...);
  }

  ConnectionType Connect(const Callback& call_back) {
    auto slot = std::make_shared<Slot<RT(Args...)>>(call_back);
    {
      std::lock_guard<std::mutex> lock(mutex_);
      slots_.emplace_back(slot);
    }

    return ConnectionType(slot, this);
  }

  bool Disconnect(const ConnectionType& conn) {
    bool find = false;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      for (auto& slot : slots_) {
        if (conn.HasSlot(slot)) {
          find = true;
          slot->Disconnect();
        }
      }
    }

    if (find) {
      ClearDisconnectedSlots();
    }
    return find;
  }

  void DisconnectAllSlots() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& slot : slots_) {
      slot->Disconnect();
    }
    slots_.clear();
  }

 private:
  template <typename Rt>
  auto CallBack(Args... args) -> Rt requires(!std::is_void<RT>::value) {
    SlotList local;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      for (auto& slot : slots_) {
        local.emplace_back(slot);
      }
    }
    Rt ret{};
    if (!local.empty()) {
      for (auto& slot : local) {
        ret = (*slot)(std::forward<Args>(args)...);
        if (!ret) {
          return ret;
        }
      }
    }
    ClearDisconnectedSlots();
    return ret;
  }

  template <typename Rt>
  auto CallBack(Args... args) -> Rt requires(std::is_void<Rt>::value) {
    SlotList local;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      for (auto& slot : slots_) {
        local.emplace_back(slot);
      }
    }

    if (!local.empty()) {
      for (auto& slot : local) {
        (*slot)(std::forward<Args>(args)...);
      }
    }

    ClearDisconnectedSlots();
  }
  Signal(const Signal&) = delete;
  Signal& operator=(const Signal&) = delete;

  void ClearDisconnectedSlots() {
    std::lock_guard<std::mutex> lock(mutex_);
    slots_.erase(
        std::remove_if(slots_.begin(), slots_.end(),
                       [](const SlotPtr& slot) { return !slot->connected(); }),
        slots_.end());
  }
  SlotList slots_;
  std::mutex mutex_;
};

template <typename RT, typename... Args>
class Connection<RT(Args...)> {
  using SlotPtr = std::shared_ptr<Slot<RT(Args...)>>;
  using SignalPtr = Signal<RT(Args...)>*;

 public:
  Connection() : slot_(nullptr), signal_(nullptr) {}
  Connection(const SlotPtr& slot, const SignalPtr& signal)
      : slot_(slot), signal_(signal) {}
  virtual ~Connection() {
    slot_ = nullptr;
    signal_ = nullptr;
  }

  Connection& operator=(const Connection& another) {
    if (this != &another) {
      this->slot_ = another.slot_;
      this->signal_ = another.signal_;
    }
    return *this;
  }

  bool HasSlot(const SlotPtr& slot) const {
    if (slot != nullptr && slot_ != nullptr) {
      return slot_.get() == slot.get();
    }
    return false;
  }

  bool IsConnected() const {
    if (slot_) {
      return slot_->connected();
    }
    return false;
  }

  bool Disconnect() {
    if (signal_ && slot_) {
      return signal_->Disconnect(*this);
    }
    return false;
  }

 private:
  SlotPtr slot_;
  SignalPtr signal_;
};

template <typename EventType, typename RT, typename... Args>
class Dispatcher<EventType, RT(Args...)> {
  using Callback = std::function<RT(Args...)>;
  using Mutex = std::shared_mutex;
  using SignalPtr = std::shared_ptr<Signal<RT(Args...)>>;
  using Map = std::map<EventType, SignalPtr>;

 public:
  bool AddListener(const EventType& event, const Callback& call_back) {
    std::lock_guard<Mutex> lockGuard(rw_lock_);
    if (!HasChannel(event)) {
      SignalPtr signal_ptr = std::make_shared<Signal<RT(Args...)>>(call_back);
      msg_listeners_.insert(std::pair<EventType, SignalPtr>(event, signal_ptr));
    } else {
      msg_listeners_[event]->Connect(call_back);
    }

    return true;
  }

  RT Dispatch(const EventType& event, Args... args) {
    return CallBack<RT>(event, std::forward<Args>(args)...);
  }

  bool RemoveListener(const EventType& event) {
    std::lock_guard<Mutex> lockGuard(rw_lock_);
    if (HasChannel(event)) {
      msg_listeners_.erase(event);
    }
    return true;
  }

  bool HasChannel(const EventType& event) {
    auto listener = msg_listeners_.find(event);
    if (listener != msg_listeners_.end()) {
      return true;
    }
    return false;
  }

 private:
  template <typename Rt>
  auto CallBack(const EventType& event, Args... args) -> Rt
      requires(std::is_void<RT>::value) {
    std::shared_lock<std::shared_mutex> lock(rw_lock_);
    if (HasChannel(event)) {
      (*msg_listeners_[event])(std::forward<Args>(args)...);
    }
  }

  template <typename Rt>
  auto CallBack(const EventType& event, Args... args) -> Rt
      requires(!std::is_void<Rt>::value) {
    std::shared_lock<std::shared_mutex> lock(rw_lock_);
    if (HasChannel(event)) {
      return (*msg_listeners_[event])(std::forward<Args>(args)...);
    }
    return RT{};
  }

  Map msg_listeners_;
  mutable Mutex rw_lock_;
};
}  // namespace XEngine
