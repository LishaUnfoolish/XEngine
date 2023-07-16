load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "pool_context",
    hdrs = ["pool_context.hpp"],
    deps = [
        "//hpcs/common:macros",
        "//hpcs/core:semaphor",
        "//hpcs/scheduler:croutine",
        "//hpcs/scheduler:processor",
        "//hpcs/scheduler:scheduler_config",
    ],
)

cc_library(
    name = "priority_context",
    hdrs = ["priority_context.hpp"],
    deps = [
        "//hpcs/common:macros",
        "//hpcs/core:semaphor",
        "//hpcs/scheduler:croutine",
        "//hpcs/scheduler:processor",
        "//hpcs/scheduler:scheduler_config",
    ],
)

cc_library(
    name = "scheduler_pool",
    hdrs = ["scheduler_pool.hpp"],
    deps = [
        "//hpcs/common:singleton",
        "//hpcs/scheduler",
        "//hpcs/scheduler:processor",
        "//hpcs/scheduler:scheduler_config",
        "//hpcs/scheduler/policy:pool_context",
    ],
)

cc_library(
    name = "scheduler_priority",
    hdrs = ["scheduler_priority.hpp"],
    deps = [
        "//hpcs/common:log",
        "//hpcs/common:singleton",
        "//hpcs/scheduler",
        "//hpcs/scheduler:processor",
        "//hpcs/scheduler:scheduler_config",
        "//hpcs/scheduler/policy:priority_context",
    ],
)
