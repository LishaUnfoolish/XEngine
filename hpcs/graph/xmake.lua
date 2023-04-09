/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
target("test_graph")
    add_cxxflags("-stdlib=libc++ ", {tools = "clang++"})
    set_kind("binary")
    add_files("test_graph.cpp")
    add_includedirs("..")

target("test_matrix")
    add_cxxflags("-stdlib=libc++ ", {tools = "clang++"})
    set_kind("binary")
    add_files("test_matrix.cpp")
    add_includedirs("..")
