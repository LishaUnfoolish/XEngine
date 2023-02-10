
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
#include <algorithm>
#include <atomic>
#include <chrono>
#include <limits>
#include <numeric>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "common/macros.hpp"
#include "graph/breadth_first_iterator.hpp"
#include "task/flow_builder.hpp"
#include "task/task.hpp"
namespace XEngine {
HAS_MEMBER_TRAITS(Run);
HAS_MEMBER_RET_TRAITS(Run);
enum class RunnerStopReason {
  RunnerOk,
  RunnerTimeLimit,
};

struct RunnerStatus {
  constexpr RunnerStatus(const RunnerStopReason& stop_reason) noexcept {
    stop_reason_ = stop_reason;
  }
  [[nodiscard]] constexpr RunnerStatus() = default;
  [[nodiscard]] constexpr bool operator()() const noexcept {
    return stop_reason_ == RunnerStopReason::RunnerOk;
  }
  [[nodiscard]] constexpr RunnerStopReason StopReason() const noexcept {
    return stop_reason_;
  }
  [[nodiscard]] constexpr decltype(auto) SetErrorMsg(
      std::string_view msg) noexcept {
    ErrorMessage = msg.data();
    return std::move(*this);
  }
  [[nodiscard]] constexpr std::string ErrorMsg() const noexcept {
    return ErrorMessage;
  }

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
  constexpr Runner(BuilderType& builder) noexcept(
      std::is_nothrow_default_constructible_v<Builder>)
      : builder_(builder) {
    Rebuild();
  }
  [[nodiscard]] constexpr ~Runner() noexcept { is_running_.clear(); }

  [[nodiscard]] constexpr bool Rebuild() noexcept {
    const auto& vec = builder_.Linearize();
    [[unlikely]] if (!vec.has_value()) { return false; }
    finished_.clear();
    running_.clear();
    for (const auto& id : vec.value()) {
      finished_.try_emplace(id, false);
      running_.try_emplace(id, false);
    }
    is_running_.test_and_set();
    return true;
  }
  template <typename RetType = RunnerStatus>
  [[nodiscard]] constexpr std::optional<RetType> Run() {
    start_time_ = std::chrono::steady_clock::now();
    struct Ret {
      Ret(const NodeIdType& id, const RetType& node_ret)
          : id_(id), node_ret_(node_ret) {}
      NodeIdType Id() const { return id_; }
      RetType RetValue() const { return node_ret_; }
      NodeIdType id_;
      RetType node_ret_;
    };

    const auto& vec = builder_.Linearize();
    [[likely]] if (vec.has_value()) {
      auto finish_count = vec.value().size();
      std::vector<std::future<Ret>> future_list{};
      while (is_running_.test()) {
        for (const auto& iter : vec.value()) {
          /* 必须是没在跑并且没有运行过
          Must not be running and must not have been running.*/

          [[likely]] if (!running_[iter] && !finished_[iter]) {
            /* 如果入度>0,就查看依赖的节点有没有跑完
            If the indegree > 0, then check to see if the dependent nodes have
            been run. */
            std::uint32_t edge_count = builder_.WorkFlows().Indegree(iter);
            [[likely]] if (edge_count > 0) {
              for (const auto& [id, state] : finished_) {
                [[likely]] if (state &&
                               builder_.WorkFlows().HasEdge(id, iter)) {
                  --edge_count;
                }
              }
            }
            [[likely]] if (edge_count == 0) {
              running_[iter] = true;
              future_list.emplace_back(
                  std::move(Async([node = &builder_.WorkFlows().GetNode(iter),
                                   iter]() -> decltype(auto) {
                    if constexpr (HasMemberName<Ret> ||
                                  CheckSmartPointerHasName<NodeType> ||
                                  CheckPointerHasName<NodeType>) {
                      DEBUG << node->Name() << std::endl;
                    }
                    static_assert(HasMemberRun<NodeType>);
                    static_assert(HasRetMemberRun<NodeType, RetType>);
                    /* run */
                    return Ret{iter, node->Run()};
                  })));
            }
          }
        }

        decltype(future_list) new_future_list{future_list.size()};
        for (auto& future : future_list) {
          [[likely]] if (future.valid()) {
            const auto& future_status =
                future.wait_for(std::chrono::milliseconds(10));
            [[likely]] if (future_status == std::future_status::ready) {
              const auto& status = future.get();
              [[unlikely]] if (!status.RetValue()()) {
                ERROR << "Failed runner by:" << status.Id();
                return status.RetValue();
              }
              finished_[status.Id()] = true;
              --finish_count;
              [[unlikely]] if (finish_count <= 0) { return status.RetValue(); }
            }
            else {
              new_future_list.emplace_back(std::move(future));
            }
          }
        }
        future_list = std::move(new_future_list);

        [[unlikely]] if (CheckLimits()) { return std::nullopt; }
      }
    }
    return std::nullopt;
  }

  template <class Rep, class Period>
  [[nodiscard]] constexpr void SetTimeLimit(
      const std::chrono::duration<Rep, Period>& limit) noexcept {
    time_limit_ = std::chrono::duration_cast<std::chrono::milliseconds>(limit);
  }
  [[nodiscard]] constexpr std::optional<RunnerStopReason>
  CheckLimits() noexcept {
    [[likely]] if (time_limit_.has_value()) {
      const auto& elapsed = std::chrono::steady_clock::now() - start_time_;
      INFO << "run time :" << elapsed << std::endl;
      [[unlikely]] if (elapsed > time_limit_.value()) {
        ERROR << "Time limit :" << elapsed << std::endl;
        return RunnerStopReason::RunnerTimeLimit;
      }
    }
    return std::nullopt;
  }

 protected:
  std::atomic_flag is_running_{};
  BuilderType& builder_{};
  std::optional<std::chrono::milliseconds> time_limit_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  std::unordered_map<NodeIdType, bool> finished_{};
  std::unordered_map<NodeIdType, bool> running_{};
};

}  // namespace XEngine