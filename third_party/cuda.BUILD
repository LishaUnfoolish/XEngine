package(default_visibility = ["//visibility:public"])

licenses(["notice"])

cc_library(
    name = "cuda",
    srcs = [
        "local/cuda/lib64/libcudart.so",
        "local/cuda/lib64/libcupti.so",
        "local/cuda/lib64/libcurand.so",
        "local/cuda/lib64/libnppc.so",
        "local/cuda/lib64/libnppial.so",
        "local/cuda/lib64/libnppidei.so",
        "local/cuda/lib64/libnppig.so",
        "local/cuda/lib64/libnpps.so",
        "local/cuda/lib64/libnvToolsExt.so",
    ],
    hdrs = glob(["local/cuda/include/**"]),
    includes = ["local/cuda/include"],
    deps = [
        "@cublas",
    ],
)
