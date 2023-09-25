
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@gmail.com
@Time: 2023/02/29
***************************/
#pragma once
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <execution>
#include <limits>
#include <list>
#include <memory_resource>
#include <numeric>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "hpcs/common/macros.hpp"
#include "hpcs/graph/breadth_first_iterator.hpp"
#include "hpcs/task/flow_builder.hpp"
#include "hpcs/task/task.hpp"

namespace XEngine {

#define RUN_OPTIMAZE

HAS_MEMBER_TRAITS(Run);
HAS_MEMBER_RET_TRAITS(Run);
enum class RunnerStopReason {
  RunnerOk,
  RunnerTimeLimit,
};

struct RunnerStatus {
  explicit RunnerStatus(const RunnerStopReason& stop_reason) noexcept {
    stop_reason_ = stop_reason;
  }
  [[nodiscard]] constexpr RunnerStatus() = default;
  // [[nodiscard]] constexpr bool operator()() const noexcept {
  //   return stop_reason_ == RunnerStopReason::RunnerOk;
  // }
  [[nodiscard]] constexpr explicit operator bool() const noexcept {
    return stop_reason_ == RunnerStopReason::RunnerOk;
  }
  [[nodiscard]] constexpr bool operator!() const noexcept {
    return stop_reason_ != RunnerStopReason::RunnerOk;
  }
  [[nodiscard]] constexpr RunnerStopReason StopReason() const noexcept {
    return stop_reason_;
  }
  [[nodiscard]] constexpr decltype(auto) SetErrorMsg(
      std::string_view msg) noexcept {
    ErrorMessage = msg.data();
    return std::move(*this);
  }
  [[nodiscard]] std::string ErrorMsg() const noexcept { return ErrorMessage; }

 private:
  RunnerStopReason stop_reason_{RunnerStopReason::RunnerTimeLimit};
  std::string ErrorMessage{};
};

template <typename Builder>
class Runner {
  using BuilderType = Builder;
  using GraphType = typename Builder::GraphType;
  using NodeType = typename GraphType::NodeType;
  using NodeIdType = typename GraphType::NodeIdType;

 public:
  Runner(const Runner&) = delete;
  Runner(Runner&&) = delete;
  Runner& operator=(const Runner&) = delete;
  Runner& operator=(Runner&&) = delete;
  constexpr explicit Runner(BuilderType& builder) noexcept(
      std::is_nothrow_default_constructible_v<Builder>)
      : builder_(builder) {}
  constexpr ~Runner() noexcept { Stop(); }
  constexpr void Stop() noexcept { running_.clear(std::memory_order_release); }

  [[nodiscard]] constexpr bool Rebuild() noexcept {
    Stop();
    if (!running_.test_and_set(std::memory_order_acquire)) [[likely]] {
      const auto& vec = builder_.Linearize();
      [[unlikely]] if (!vec.has_value()) {
        Stop();
        return false;
      }
      /* 统计相邻节点依赖关系 */
      dep_table_.resize(vec.value().size(), NodeIdNone);
      for (int i = 0; i < vec.value().size(); i++) {
        /* 反向遍历,找到临近依赖节点 */
        for (int f_i = i - 1; f_i >= 0; f_i--) {
          /* 判断是有边 */
          if (builder_.WorkFlows().HasEdge(vec.value()[f_i], vec.value()[i]))
              [[likely]] {
            dep_table_[vec.value()[i]] = vec.value()[f_i];
            break;
          }
        }
      }
      work_flows_.assign(vec.value().begin(), vec.value().end());
    }
    return true;
  }

  /* 并行优化 */
  template <typename RetType = RunnerStatus, typename... Args>
  [[nodiscard]] constexpr std::optional<RetType> Run(Args&&... args) {
    uint32_t finish_count = work_flows_.size();

    std::vector<NodeIdType> finish(finish_count, 0);
    std::list<NodeIdType> work_flows;
    work_flows.assign(work_flows_.begin(), work_flows_.end());
    std::list<std::pair<NodeIdType, RunnerFutureType<RetType>>> future_list;
    while (finish_count > 0 && running_.test(std::memory_order_acquire)) {
      for (auto it = work_flows.begin(); it != work_flows.end();) {
        NodeIdType dep_id = dep_table_[*it];
        if (dep_id == NodeIdNone || finish[dep_id] != 0) [[likely]] {
          future_list.emplace_back(
              *it,
              std::move(Async(&Runner<BuilderType>::Execl<RetType, Args...>,
                              this, &(builder_.WorkFlows().GetNode(*it)),
                              std::forward<Args>(args)...)));
          it = work_flows.erase(it);
        } else {
          ++it;
        }
      }

      for (auto pair = future_list.begin(); pair != future_list.end();) {
        [[likely]] if (pair->second.wait_for(std::chrono::microseconds(1)) ==
                       ReadyValue) {
          auto ret = pair->second.get();
          [[unlikely]] if (!ret) {
            XERROR << "Failed runner by:" << pair->first;
            Stop();
            return ret;
          }
          --finish_count;
          finish[pair->first] = 1;
          pair = future_list.erase(pair);
        } else {
          ++pair;
        }
      }
    }
    return std::nullopt;
  }

  template <typename RetType, typename... Args>
  [[nodiscard]] constexpr RetType Execl(std::function<NodeType>* node_data,
                                        Args&&... args)
    requires(std::is_function_v<NodeType>)
  {
    return (*node_data)(std::forward<Args>(args)...);
  }

  template <typename RetType>
  [[nodiscard]] constexpr RetType Execl(NodeType* node_data)
    requires(!std::is_function_v<NodeType>)
  {
    // if constexpr (HasMemberName<NodeType> ||
    //               CheckSmartPointerHasName<NodeType> ||
    //               CheckPointerHasName<NodeType>) {
    //   XDEBUG << node_data->Name() << std::endl;
    // }
    static_assert(HasMemberRun<NodeType>);
    static_assert(HasRetMemberRun<NodeType, RetType>);
    /* run */
    return node_data->Run();
  }

  template <class Rep, class Period>
  constexpr void SetTimeLimit(
      const std::chrono::duration<Rep, Period>& limit) noexcept {
    time_limit_ = std::chrono::duration_cast<std::chrono::milliseconds>(limit);
  }
  [[nodiscard]] constexpr std::optional<RunnerStopReason>
  CheckLimits() noexcept {
    [[likely]] if (time_limit_.has_value()) [[likely]] {
      const auto& elapsed = std::chrono::steady_clock::now() - start_time_;
      XINFO << "run time :" << std::to_string(elapsed.count()) << std::endl;
      [[unlikely]] if (elapsed > time_limit_.value()) [[unlikely]] {
        XINFO << "run time :" << std::to_string(elapsed.count()) << std::endl;
        return RunnerStopReason::RunnerTimeLimit;
      }
    }
    return std::nullopt;
  }

 private:
  BuilderType& builder_{};
  std::optional<std::chrono::milliseconds> time_limit_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  std::atomic_flag running_{};
  // std::atomic_bool running_{false};
  std::list<NodeIdType> work_flows_{};
  std::vector<NodeIdType> dep_table_{};
};

}  // namespace XEngine
