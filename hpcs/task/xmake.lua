target("test_flow_builder")
    add_cxxflags("-stdlib=libc++ ", {tools = "clang++"})
    set_kind("binary")
    add_files("test_flow_builder.cpp")
    add_includedirs("../..")

target("test_runner")
    add_cxxflags("-stdlib=libc++ ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_files("test_runner.cpp")
    add_includedirs("../..")


