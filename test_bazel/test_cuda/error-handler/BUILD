load("//bazeltools/rules/cuda_library:cuda_library.bzl", "cuda_library")

cc_library(
    name = "utils_lib",
    srcs = [
        "utils.cpp",
    ],
    hdrs = [
        "utils.hpp",
    ],
    deps = [
        "@cuda",
    ],
)

cuda_library(
    name = "cuda_lib",
    srcs = [
        "matmul_gpu_basic.cu",
    ],
    hdrs = [
        "matmul.hpp",
    ],
    deps = [
        ":utils_lib",
        "@cuda",
    ],
)

cc_library(
    name = "cc_lib",
    srcs = [
        "matmul_cpu.cpp",
        ":cuda_lib",
    ],
    hdrs = [
        "matmul.hpp",
        "timer.hpp",
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
