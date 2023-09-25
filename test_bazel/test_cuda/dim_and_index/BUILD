load("//bazeltools/rules/cuda_library:cuda_library.bzl", "cuda_library")

cuda_library(
    name = "print_index_lib",
    srcs = [
        "print_index.cu",
    ],
    hdrs = [
        "print_index.h",
    ],
    deps = [
        "@cuda",
    ],
)

cc_library(
    name = "print_indexs",
    srcs = [
        ":print_index_lib",
    ],
    hdrs = [
        "print_index.h",
    ],
    linkstatic = 1,
    deps = [
        "@cuda",
    ],
)

cc_test(
    name = "main",
    size = "medium",
    srcs = ["main.cpp"],
    deps = [
        ":print_indexs",
    ],
)
