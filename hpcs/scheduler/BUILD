load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "croutine",
    hdrs = ["croutine.hpp"],
)

cc_library(
    name = "processor",
    hdrs = ["processor.hpp"],
)

cc_library(
    name = "scheduler_config",
    hdrs = ["scheduler_config.hpp"],
    deps = [
        "//hpcs/common:log",
        "//hpcs/common:singleton",
        "//hpcs/scheduler",
        "//hpcs/scheduler:croutine",
        "//third_party/nlohmann:json",
    ],
)

cc_library(
    name = "scheduler_policy",
    hdrs = ["scheduler_policy.hpp"],
    deps = [
        "//hpcs/common:singleton",
        "//hpcs/core:spin_lock",
        "//hpcs/scheduler",
        "//hpcs/scheduler/policy:scheduler_pool",
        "//hpcs/scheduler/policy:scheduler_priority",
    ],
)

cc_library(
    name = "scheduler",
    hdrs = ["scheduler.hpp"],
)

cc_library(
    name = "awaitable_traits",
    hdrs = ["awaitable_traits.hpp"],
)
