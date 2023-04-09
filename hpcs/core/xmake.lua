/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
target("test_signal")
    add_cxxflags("-stdlib=libc++ ", {tools = "clang++"})
    set_kind("binary")
    add_files("test_signal.cpp")
    add_includedirs("..")