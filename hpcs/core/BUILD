load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "hpcs_core_headers",
    srcs = glob([
        "*.hpp",
    ]),
)

cc_library(
    name = "semaphor",
    hdrs = ["semaphor.hpp"],
)

cc_library(
    name = "signal",
    hdrs = ["signal.hpp"],
)

cc_library(
    name = "spin_lock",
    hdrs = ["spin_lock.hpp"],
)

cc_binary(
    name = "test_signal",
    srcs = ["test_signal.cpp"],
    deps = [
        "//hpcs/common:catch",
        "//hpcs/core:signal",
    ],
)
