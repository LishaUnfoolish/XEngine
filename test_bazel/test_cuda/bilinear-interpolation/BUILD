load("//bazeltools/rules/cuda_library:cuda_library.bzl", "cuda_library")

cc_library(
    name = "utils_lib",
    srcs = [
        "preprocess.cpp",
        "timer.cpp",
        "utils.cpp",
    ],
    hdrs = [
        "preprocess.hpp",
        "timer.hpp",
        "utils.hpp",
    ],
    deps = [
        "@cuda",
        "@opencv4//:core",
        "@opencv4//:highgui",
    ],
)

cuda_library(
    name = "cuda_lib",
    srcs = [
        "preprocess.cu",
    ],
    hdrs = [
    ],
    deps = [
        ":utils_lib",
        "@cuda",
        "@opencv4//:core",
    ],
)

cc_library(
    name = "cc_lib",
    srcs = [
        ":cuda_lib",
    ],
    hdrs = [
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
