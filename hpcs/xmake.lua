-- includes("include", {global=true})
target("hocs")
    add_cxxflags("-stdlib=libc++", {tools = "clang++"})
    add_includedirs(".")