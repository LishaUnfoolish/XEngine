/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2022/11/20
@Desc: 信号与槽接口的测试用例
***************************/

#include <iostream>
#include <memory>
#include <utility>

#include "common/catch.hpp"
#include "core/signal.hpp"
class MyInt {
 public:
  template <typename T>
  MyInt(const T value) : value(value) {}
  std::uint32_t getValue() const { return value; }
  bool operator==(const MyInt& b) const noexcept {
    return getValue() == b.getValue();
  }
  bool operator<(const MyInt& b) const noexcept {
    return getValue() < b.getValue();
  }

 private:
  std::uint32_t value;
};

namespace std {
template <>
struct hash<MyInt> {
  std::size_t operator()(const MyInt& value) const noexcept {
    return std::hash<std::uint32_t>()(value.getValue());
  }
};
}  // namespace std

TEST_CASE("test_slot") {
  std::uint32_t sum = 0;
  std::uint32_t lhs = 1, rhs = 2;
  XEngine::Slot<bool(std::uint32_t, std::uint32_t)> slot_a(
      [&sum](std::uint32_t lhs, std::uint32_t rhs) -> bool {
        sum = lhs + rhs;
        return false;
      });
  slot_a.connected();
  REQUIRE(!slot_a(lhs, rhs));
  REQUIRE(sum == lhs + rhs);

  XEngine::Slot<void(std::uint32_t, std::uint32_t)> slot_b(
      [&sum](std::uint32_t lhs, std::uint32_t rhs) -> void {});
  slot_b.connected();
  REQUIRE(std::is_same<void, decltype(slot_b(lhs, rhs))>::value);

  XEngine::Slot<std::string(std::uint32_t, std::uint32_t)> slot_c(
      [&sum](std::uint32_t lhs, std::uint32_t rhs) -> std::string {
        return std::to_string(lhs) + std::to_string(rhs);
      });
  slot_c.connected();
  REQUIRE(slot_c(lhs, rhs) == std::to_string(lhs) + std::to_string(rhs));
}

TEST_CASE("test_signal") {
  std::uint32_t step{};
  XEngine::Signal<bool(std::uint32_t, std::uint32_t)> sig;
  sig.Connect([&step](std::uint32_t lhs, std::uint32_t rhs) -> bool {
    ++step;
    return true;
  });
  sig.Connect([&step](std::uint32_t lhs, std::uint32_t rhs) -> bool {
    ++step;
    return true;
  });
  sig.Connect([&step](std::uint32_t lhs, std::uint32_t rhs) -> bool {
    ++step;
    return true;
  });
  REQUIRE(step == 0);
  REQUIRE(sig(1, 2));
  REQUIRE(step == 3);

  step = 0;
  sig.Connect(
      [&step](std::uint32_t lhs, std::uint32_t rhs) -> bool { return false; });
  sig.Connect([&step](std::uint32_t lhs, std::uint32_t rhs) -> bool {
    ++step;
    return true;
  });
  REQUIRE(!sig(1, 2));
  REQUIRE(step == 3);
}

TEST_CASE("test_dispatcher_singnal") {
  std::uint32_t sum{};
  XEngine::Dispatcher<std::uint32_t, bool(std::uint32_t, std::uint32_t)>
      dispatcher_a;
  dispatcher_a.AddListener(1, [&sum](std::uint32_t a, std::uint32_t b) -> bool {
    ++sum;
    return false;
  });
  dispatcher_a.AddListener(2, [&sum](std::uint32_t a, std::uint32_t b) -> bool {
    ++sum;
    return true;
  });
  REQUIRE(!dispatcher_a.Dispatch(1, 1, 1));
  REQUIRE(sum == 1);
  REQUIRE(dispatcher_a.Dispatch(2, 1, 1));
  REQUIRE(sum == 2);

  std::string str{};
  XEngine::Dispatcher<std::string, bool(std::uint32_t, std::uint32_t)>
      dispatcher_b;
  dispatcher_b.AddListener("1",
                           [&str](std::uint32_t a, std::uint32_t b) -> bool {
                             str = std::to_string(a) + std::to_string(b);
                             return false;
                           });
  dispatcher_b.AddListener("2",
                           [&str](std::uint32_t a, std::uint32_t b) -> bool {
                             str = std::to_string(a) + std::to_string(b);
                             return true;
                           });

  REQUIRE(!dispatcher_b.Dispatch("1", 1, 1));
  REQUIRE(str == std::to_string(1) + std::to_string(1));
  REQUIRE(dispatcher_b.Dispatch("2", 2, 2));
  REQUIRE(str == std::to_string(2) + std::to_string(2));

  XEngine::Dispatcher<std::string, void(std::uint32_t, std::uint32_t)>
      dispatcher_c;
  dispatcher_c.AddListener("1",
                           [&](std::uint32_t a, std::uint32_t b) -> void {});
  dispatcher_c.AddListener("2",
                           [&](std::uint32_t a, std::uint32_t b) -> void {});
  REQUIRE(
      std::is_same<void, decltype(dispatcher_c.Dispatch("1", 1, 1))>::value);
  REQUIRE(
      std::is_same<void, decltype(dispatcher_c.Dispatch("2", 2, 2))>::value);

  sum = 0;
  XEngine::Dispatcher<MyInt, void(std::uint32_t, std::uint32_t)> dispatcher_d;
  dispatcher_d.AddListener(
      MyInt(1), [&sum](std::uint32_t a, std::uint32_t b) -> void { ++sum; });
  dispatcher_d.AddListener(
      MyInt(1), [&sum](std::uint32_t a, std::uint32_t b) -> void { ++sum; });
  dispatcher_d.AddListener(
      MyInt(1), [&sum](std::uint32_t a, std::uint32_t b) -> void { ++sum; });
  dispatcher_d.Dispatch(MyInt(1), 1, 1);
  REQUIRE(sum == 3);

  dispatcher_d.Dispatch(MyInt(1), 1, 1);
  REQUIRE(sum == 6);
}
