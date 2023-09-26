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

cc_test(
    name = "main",
    size = "medium",
    srcs = ["main.cpp"],
    deps = [
        ":utils_lib",
    ],
)
