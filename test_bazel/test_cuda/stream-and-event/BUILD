load("//bazeltools/rules/cuda_library:cuda_library.bzl", "cuda_library")

cc_library(
    name = "utils_lib",
    srcs = [
        "timer.cpp",
        "utils.cpp",
    ],
    hdrs = [
        "timer.hpp",
        "utils.hpp",
    ],
    deps = [
        "@cuda",
    ],
)

cuda_library(
    name = "cuda_lib",
    srcs = [
        "gelu.cu",
        "stream.cu",
    ],
    hdrs = [
        "gelu.hpp",
        "stream.hpp",
    ],
    deps = [
        ":utils_lib",
        "@cuda",
    ],
)

cc_library(
    name = "cc_lib",
    srcs = [
        ":cuda_lib",
    ],
    hdrs = [
        "gelu.hpp",
        "stream.hpp",
    ],
    deps = [
        "@cuda",
    ],
)

cc_test(
    name = "main",
    size = "medium",
    srcs = ["main.cpp"],
    deps = [
        ":cc_lib",
        ":utils_lib",
        "@cuda",
    ],
)
