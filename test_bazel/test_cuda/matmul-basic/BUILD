load("//bazeltools/rules/cuda_library:cuda_library.bzl", "cuda_library")

cuda_library(
    name = "cuda_lib",
    srcs = [
        "matmul_gpu_basic.cu",
    ],
    hdrs = [
        "matmul.hpp",
    ],
    deps = [
        "@cuda",
    ],
)

cc_library(
    name = "utils_lib",
    srcs = [
        "matmul_cpu.cpp",
        "utils.cpp",
        ":cuda_lib",
    ],
    hdrs = [
        "matmul.hpp",
        "timer.hpp",
        "utils.hpp",
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
        ":utils_lib",
    ],
)
