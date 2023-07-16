package(default_visibility = ["//visibility:public"])

filegroup(
    name = "hpcs_common_headers",
    srcs = glob([
        "*.hpp",
    ]),
)

cc_library(
    name = "catch",
    hdrs = ["catch.hpp"],
)

cc_library(
    name = "log",
    hdrs = ["log.hpp"],
)

cc_library(
    name = "macros",
    hdrs = ["macros.hpp"],
)

cc_library(
    name = "singleton",
    hdrs = ["singleton.hpp"],
    deps = [
        "//hpcs/common:macros"
    ],
)

cc_library(
    name = "futex",
    hdrs = ["futex.hpp"],
    deps = [
        "//hpcs/common:macros"
    ],
)