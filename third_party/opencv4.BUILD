package(default_visibility = ["//visibility:public"])

cc_library(
    name = "aruco",
    srcs = [
        "lib/libopencv_aruco.so.4.5.5",
    ],
    deps = [
        ":calib3d",
        ":core",
        ":features2d",
        ":flann",
        ":imgproc",
    ],
)

cc_library(
    name = "calib3d",
    srcs = [
        "lib/libopencv_calib3d.so.4.5.5",
    ],
    deps = [
        ":core",
        ":features2d",
        ":flann",
        ":imgproc",
    ],
)

cc_library(
    name = "core",
    srcs = [
        "lib/libopencv_core.so.4.5.5",
    ],
    hdrs = glob(["include/opencv2/*.h*","include/opencv2/**/*.h*"]),
    includes = [
        "include",
    ],
    linkopts = [
        "-pthread",
    ],
    deps = [
        # "@zlib",
    ],
)

cc_library(
    name = "imgcodecs",
    srcs = [
        "lib/libopencv_imgcodecs.so.4.5.5",
    ],
    deps = [
        ":imgproc",
    ],
)

cc_library(
    name = "imgproc",
    srcs = [
        "lib/libopencv_imgproc.so.4.5.5",
    ],
    deps = [
        ":core",
    ],
)

cc_library(
    name = "features2d",
    srcs = [
        "lib/libopencv_features2d.so.4.5.5",
    ],
    deps = [
        ":core",
        ":imgproc",
    ],
)

cc_library(
    name = "flann",
    srcs = [
        "lib/libopencv_flann.so.4.5.5",
    ],
    deps = [
        ":core",
    ],
)

cc_library(
    name = "ml",
    srcs = [
        "lib/libopencv_ml.so.4.5.5",
    ],
    deps = [
        ":core",
    ],
)

cc_library(
    name = "video",
    srcs = [
        "lib/libopencv_video.so.4.5.5",
    ],
    deps = [
        ":core",
    ],
)

cc_library(
    name = "cuda",
    srcs = glob(["lib/libopencv_cuda*.so*"]),
    deps = [
        ":core",
    ],
)

cc_library(
    name = "highgui",
    srcs = [
        "lib/libopencv_highgui.so.4.5.5",
        # "lib/libjasper.so.4",
    ],
    linkopts = [
        "-ljpeg",
        "-ltiff",
        "-lpng",
    ],
    deps = [
        ":core",
        ":imgcodecs",
        ":imgproc",
    ],
)
