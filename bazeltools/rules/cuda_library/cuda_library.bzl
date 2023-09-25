load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain")

_CUDA_SRCS = [
    ".cu",
    ".cc",
    ".cpp",
]

_CUDA_HEADERS = [
    ".h",
    ".hpp",
    ".cu",
    ".cuh",
]

def impl(ctx):
    package_path = ctx.label.package.replace("_", "_U").replace("/", "_S")
    package_name = ctx.label.name.replace("_", "_U")
    cuda_lib_file = ctx.actions.declare_file("lib{}_S{}.so".format(package_path, package_name))

    inputs = []
    inputs += ctx.files.deps
    inputs += ctx.files.srcs
    inputs += ctx.files.hdrs
    for dep in ctx.attr.deps:
        inputs += dep[CcInfo].compilation_context.headers.to_list()
    flags = []
    for dep in ctx.attr.deps:
        flags += ["-I" + d for d in dep[CcInfo].compilation_context.includes.to_list()]
        flags += ["-I" + d for d in dep[CcInfo].compilation_context.quote_includes.to_list()]
        flags += ["-I" + d for d in dep[CcInfo].compilation_context.system_includes.to_list()]
    args = [f.path for f in ctx.files.srcs]
    args += [
        "-D__CUDACC__",
        "-DNVCC",
        "-ccbin /usr/bin/gcc-11",
        "--compiler-options ,\"-fopenmp\",\"-O3\",\"-DNVCC\",\"-fPIC\",\"-DNDEBUG\"",
        "--link",
        "-L/usr/local/cuda/targets/x86_64-linux/lib -lcudart",
        "--shared",
        "-arch=sm_60",
        "-gencode=arch=compute_60,code=sm_60",
        "-gencode=arch=compute_61,code=sm_61",
        "-gencode=arch=compute_70,code=sm_70",
        "-gencode=arch=compute_70,code=compute_70",
        "-gencode=arch=compute_75,code=sm_75",
        "-gencode=arch=compute_75,code=compute_75",
        "-gencode=arch=compute_80,code=sm_80",
        "-gencode=arch=compute_80,code=compute_80",
        "-gencode=arch=compute_86,code=sm_86",
        "-gencode=arch=compute_86,code=compute_86",
        "-std=c++17",
        "-Xcudafe --display_error_number",
    ]
    args += ctx.attr.flags
    args += ["--output-file " + cuda_lib_file.path]
    args += ["-I."]
    args += flags
    command = "nvcc %s" % (" ".join(args))
    ctx.actions.run_shell(
        inputs = inputs,
        outputs = [cuda_lib_file],
        env = {"PATH": "/usr/local/cuda/bin:/usr/local/bin:/usr/bin:/bin"},
        command = command,
    )
    return [DefaultInfo(files = depset([cuda_lib_file])), CcInfo()]
cuda_library = rule(
    attrs = {
        "hdrs": attr.label_list(allow_files = _CUDA_HEADERS),
        "srcs": attr.label_list(allow_files = _CUDA_SRCS),
        "deps": attr.label_list(),
        "flags": attr.string_list(),
    },
    implementation = impl,
)
