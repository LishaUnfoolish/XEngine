load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "flow_builder",
    hdrs = ["flow_builder.hpp"],
    deps = [
        "//hpcs/graph",
        "//hpcs/graph:topology_sort",
    ],
)

cc_library(
    name = "runner",
    hdrs = ["runner.hpp"],
    deps = [
        "//hpcs/common:macros",
        "//hpcs/graph:breadth_first_iterator",
        "//hpcs/task",
        "//hpcs/task:flow_builder",
    ],
)

cc_library(
    name = "task",
    hdrs = ["task.hpp"],
    deps = [
        "//hpcs/common:singleton",
        "//hpcs/scheduler:scheduler_policy",
    ],
)

cc_binary(
    name = "test_flow_builder",
    srcs = [
        "test_flow_builder.cpp",
    ],
    deps = [
        "//hpcs/common:catch",
        "//hpcs/graph",
        "//hpcs/graph:dense_graph",
        "//hpcs/task:flow_builder",
    ],
)

cc_binary(
    name = "test_runner",
    srcs = [
        "test_runner.cpp",
    ],
    deps = [
        "//hpcs/common:catch",
        "//hpcs/graph",
        "//hpcs/graph:dense_graph",
        "//hpcs/task:flow_builder",
        "//hpcs/task:runner",
    ],
)
