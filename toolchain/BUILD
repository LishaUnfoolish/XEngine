load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
# ipc-toolsuite
load("@bazel_tools//tools/cpp:unix_cc_toolchain_config.bzl", "cc_toolchain_config")
package(default_visibility = ["//visibility:public"])
filegroup(
    name = "empty",
    srcs = [],
)
cc_toolchain_suite(
    name = "cc_toolchain_suite",
    toolchains = {
        "ipc|clang": ":clang",
    },
)
cc_toolchain(
    name = "clang",
    all_files = ":empty",
    ar_files = ":empty",
    as_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    module_map = None,
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 1,
    toolchain_config = ":clang-ipc",
    toolchain_identifier = "clang-ipc",
)

cc_toolchain_config(
    name = "clang-ipc",
    abi_libc_version = "local",
    abi_version = "local",
    compile_flags = [
        "-U_FORTIFY_SOURCE",
        "-fstack-protector",
        "-Wall",
        "-Wthread-safety",
        "-Wself-assign",
        "-Wunused-but-set-parameter",
        "-fcolor-diagnostics",
        "-fno-omit-frame-pointer",
    ],
    compiler = "clang",
    coverage_compile_flags = [
        "--coverage",
    ],
    coverage_link_flags = [
        "--coverage",
    ],
    cpu = "ipc",
    cxx_builtin_include_directories = [
        "/usr/local/include/c++/v1",
        "/usr/local/lib/clang/15.0.7/include",
        "/usr/local/include",
        "/usr/include/x86_64-linux-gnu",
        "/usr/include",
        "/usr/local/lib/clang/15.0.7/share",
        "/usr/include/c++/11",
        "/usr/include/x86_64-linux-gnu/c++/11",
        "/usr/include/c++/11/backward",
    ],
    cxx_flags = [
        "-std=c++2b",
        "-stdlib=libstdc++",
        "-Wno-error=unused-command-line-argument",
    ],
    dbg_compile_flags = [
        "-g3",
        "-fprofile-instr-generate",
        "-fcoverage-mapping",
    ],
    host_system_name = "local",
    link_flags = [
        "-Wno-unused-command-line-argument",
        "-fuse-ld=/usr/local/bin/ld.lld",
        "-Wl,-no-as-needed",
        "-Wl,-z,relro,-z,now",
        "-B/usr/local/bin",
        "-lm",
        "-lstdc++",
    ],
    link_libs = [],
    opt_compile_flags = [
        "-g0",
        "-O2",
        "-D_FORTIFY_SOURCE=1",
        "-DNDEBUG",
        "-ffunction-sections",
        "-fdata-sections",
    ],
    opt_link_flags = [
        "-Wl,--gc-sections",
    ],
    supports_start_end_lib = True,
    target_libc = "local",
    target_system_name = "local",
    tool_paths = {
        "ar": "/usr/local/bin/llvm-ar",
        "ld": "/usr/local/bin/lld",
        "cpp": "/usr/local/bin/clang-cpp",
        "gcc": "/usr/local/bin/clang",
        "dwp": "/usr/local/bin/llvm-dwp",
        "llvm-cov": "/usr/local/bin/llvm-cov",
        "nm": "/usr/local/bin/llvm-nm",
        "objcopy": "/usr/local/bin/llvm-objcopy",
        "objdump": "/usr/local/bin/llvm-objdump",
        "strip": "/usr/local/bin/llvm-strip",
        "gcov": "/usr/bin/gcov",
    },
    toolchain_identifier = "clang-ipc",
    unfiltered_compile_flags = [
        "-no-canonical-prefixes",
        "-Wno-builtin-macro-redefined",
        "-D__DATE__=\"redacted\"",
        "-D__TIMESTAMP__=\"redacted\"",
        "-D__TIME__=\"redacted\"",
    ],
)
