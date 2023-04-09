
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
target("test_scheduler")
    add_cxxflags("-stdlib=libc++ ", {tools = "clang++"})
    add_cxxflags("-lpthread", {global=true})
    set_kind("binary")
    add_files("test_scheduler.cpp")
    add_includedirs("..")
    add_packages("nlohmann_json")

includes("policy/scheduler", {global=true})