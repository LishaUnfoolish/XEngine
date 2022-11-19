-- includes("include", {global=true})

set_project("XEngine")
set_version("1.0.0")
set_languages("c++20")

target("benchmark_test")
    add_cxxflags("-stdlib=libc++", {tools = "clang++"})
    set_kind("binary")
    add_files("benchmark_test/**.cpp")
    add_includedirs(".")