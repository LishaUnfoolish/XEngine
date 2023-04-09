/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
target("hpcs_benchmark")
    add_cxxflags("-stdlib=libc++", {tools = "clang++"})
    set_kind("binary")
    add_files("**.cpp")
    add_includedirs("../")