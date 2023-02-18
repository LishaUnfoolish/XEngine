
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

/*
The Runner class is a template class that is designed to execute a set of tasks
in parallel. The template parameter Builder is the type of the object that
builds the directed acyclic graph (DAG) representing the dependencies between
tasks.

The Runner class has a private member variable builder_ of type BuilderType,
which is a reference to the object that builds the DAG. The class also has three
private member variables: finished_, running_, and is_running_. finished_ is an
std::unordered_map that keeps track of whether a task has been executed.
running_ is also an std::unordered_map that keeps track of whether a task is
currently being executed. is_running_ is an std::atomic_flag that is used to
stop the execution of the tasks.

The Runner class has several public member functions. The Stop function sets the
is_running_ flag to false, which stops the execution of the tasks. The Rebuild
function rebuilds the DAG based on the current state of the builder object. The
Run function executes the tasks in parallel. The Run function takes a variable
number of arguments that are passed to the Run method of each task. The Run
function returns an std::optional that contains a RunnerStatus object that
describes the reason for stopping the execution of the tasks. If the execution
is stopped because of an error, the RunnerStatus object also contains an error
message. */

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
  // [[nodiscard]] constexpr bool operator()() const noexcept {
  //   return stop_reason_ == RunnerStopReason::RunnerOk;
  // }
  [[nodiscard]] constexpr operator bool() const noexcept {
    return stop_reason_ == RunnerStopReason::RunnerOk;
  }
  [[nodiscard]] constexpr bool operator!() const noexcept {
    return stop_reason_ == RunnerStopReason::RunnerOk ? false : true;
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
      : builder_(builder) {}
  [[nodiscard]] constexpr ~Runner() noexcept { Stop(); }
  [[nodiscard]] constexpr void Stop() noexcept { is_running_.clear(); }
  [[nodiscard]] constexpr std::optional<std::vector<NodeIdType>> Rebuild(
      BuilderType& builder) noexcept {
    const auto& vec = builder.Linearize();
    [[unlikely]] if (!vec.has_value()) { return std::nullopt; }
    finished_.clear();
    running_.clear();
    for (const auto& id : vec.value()) {
      finished_.try_emplace(id, false);
      running_.try_emplace(id, false);
    }
    is_running_.test_and_set();
    return vec;
  }

  template <typename RetType = RunnerStatus, typename... Args>
  [[nodiscard]] constexpr std::optional<RetType> Run(Args&&... args) {
    start_time_ = std::chrono::steady_clock::now();
    struct RetWarpper {
      RetWarpper(const NodeIdType& id, const RetType& node_ret)
          : id_(id), node_ret_(node_ret) {}
      NodeIdType Id() const { return id_; }
      RetType RetValue() const { return node_ret_; }
      NodeIdType id_;
      RetType node_ret_;
    };
    const auto& work_flows = Rebuild(builder_);
    if (!work_flows) {
      ERROR << "Failed to invoked Rebuild\n";
      return std::nullopt;
    }

    auto finish_count = work_flows.value().size();
    std::vector<std::future<std::optional<RetWarpper>>> future_list{};
    while (is_running_.test()) {
      for (const auto& iter : work_flows.value()) {
        /* 必须是没在跑并且没有运行过
        Must not be running and must not have been running.*/
        [[likely]] if (!running_[iter] && !finished_[iter]) {
          /* 如果入度>0,就查看依赖的节点有没有跑完
          If the indegree > 0, then check to see if the dependent nodes have
          been run. */
          std::uint32_t edge_count = builder_.WorkFlows().Indegree(iter);
          [[likely]] if (edge_count > 0) {
            for (const auto& [id, state] : finished_) {
              [[likely]] if (state && builder_.WorkFlows().HasEdge(id, iter)) {
                --edge_count;
              }
            }
          }
          [[likely]] if (edge_count == 0) {
            running_[iter] = true;
            future_list.emplace_back(std::move(
                Async(&Runner<BuilderType>::Execl<RetType, RetWarpper, Args...>,
                      this, iter, &(builder_.WorkFlows().GetNode(iter)),
                      std::forward<Args>(args)...)));
          }
        }
      }

      decltype(future_list) new_future_list{future_list.size()};
      for (auto& future : future_list) {
        [[likely]] if (future.valid()) {
          const auto& future_status =
              future.wait_for(std::chrono::milliseconds(10));
          [[likely]] if (future_status == std::future_status::ready) {
            auto status = future.get();
            [[likely]] if (status.has_value()) {
              [[unlikely]] if (!status.value().RetValue()) {
                ERROR << "Failed runner by:" << status.value().Id();
                return status.value().RetValue();
              }
              finished_[status.value().Id()] = true;
              --finish_count;
              [[unlikely]] if (finish_count <= 0) {
                return status.value().RetValue();
              }
            }
          }
          else {
            new_future_list.emplace_back(std::move(future));
          }
        }
      }
      future_list = std::move(new_future_list);
      [[unlikely]] if (CheckLimits()) { return std::nullopt; }
    }
    return std::nullopt;
  }

  template <typename RetType, typename RetWarpper, typename... Args>
  [[nodiscard]] constexpr std::optional<RetWarpper> Execl(
      const NodeIdType& id, std::function<NodeType>* node_data,
      Args&&... args) requires(std::is_function_v<NodeType>) {
    return RetWarpper{id, (*node_data)(std::forward<Args>(args)...)};
  }

  template <typename RetType, typename RetWarpper>
  [[nodiscard]] constexpr std::optional<RetWarpper> Execl(
      const NodeIdType& id,
      NodeType* node_data) requires(!std::is_function_v<NodeType>) {
    if constexpr (HasMemberName<RetWarpper> ||
                  CheckSmartPointerHasName<NodeType> ||
                  CheckPointerHasName<NodeType>) {
      DEBUG << node_data->Name() << std::endl;
    }
    static_assert(HasMemberRun<NodeType>);
    static_assert(HasRetMemberRun<NodeType, RetType>);
    /* run */
    return RetWarpper{id, node_data->Run()};
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
  BuilderType& builder_{};
  std::optional<std::chrono::milliseconds> time_limit_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  std::unordered_map<NodeIdType, bool> finished_{};
  std::unordered_map<NodeIdType, bool> running_{};
  std::atomic_flag is_running_{};
};

}  // namespace XEngine
