/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/04/10
***************************/
includes("third_party", {global=true})
includes("hpcs", {global=true})
set_project("XEngine")
set_version("1.0.0")
set_languages("c++2b")
add_cxxflags("-O2", {global=true})
add_cxxflags("-lpthread", {global=true})
add_includedirs("third_party", {global=true})
add_includedirs("hpcs", {global=true})