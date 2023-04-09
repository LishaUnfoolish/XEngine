target("coroutine_lib")
    set_kind("shared")
    -- 源代码文件
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    add_includedirs("include")
    add_files("source/**.cpp")
    add_files("source/task.cpp")
    add_files("source/executors/*.cpp")
    add_files("source/results/impl/*.cpp")
    add_files("source/results/*.cpp")
    add_files("source/runtime/*.cpp")
    add_files("source/threads/*.cpp")
    add_files("source/timers/*.cpp")

    add_includedirs("include/concurrencpp")
    add_includedirs("include/concurrencpp/coroutines")
    add_includedirs("include/concurrencpp/executors")
    add_includedirs("include/concurrencpp/results/impl")
    add_includedirs("include/concurrencpp/results")
    add_includedirs("include/concurrencpp/runtime")
    add_includedirs("include/concurrencpp/threads")
    add_includedirs("include/concurrencpp/timers")
    add_includedirs("include/concurrencpp/utils")
    add_includedirs("ut/include")

target("ut_lib")
    set_kind("shared")
    -- 源代码文件
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    --[[ ut include dir ]]
    add_includedirs("ut/include/infra")
    add_includedirs("ut/include/utils")
    add_includedirs("include")
    add_includedirs("ut/include")
    --[[ ut src]]
    add_files("ut/source/infra/*.cpp")
    add_files("ut/source/utils/*.cpp")

target("coroutine_tests")
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_deps("ut_lib")
    add_includedirs("include")
    add_includedirs("ut/include")
    
    --[[ taget main ]]
    add_files("ut/source/tests/coroutine_tests/coroutine_tests.cpp")


target("coroutine_promise_tests")
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_deps("ut_lib")
    add_includedirs("include")
    add_includedirs("ut/include")
    
    --[[ taget main ]]
    add_files("ut/source/tests/coroutine_tests/coroutine_promise_tests.cpp")


target("thread_executor_tests")
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_deps("ut_lib")
    add_includedirs("include")
    add_includedirs("ut/include")

    --[[ taget main ]]
    add_files("ut/source/tests/executor_tests/thread_executor_tests.cpp")

target("tasks_tests")
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_deps("ut_lib")
    add_includedirs("include")
    add_includedirs("ut/include")

    --[[ taget main ]]
    add_files("ut/source/tests/task_tests.cpp")

target("inline_executor_tests")
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_deps("ut_lib")
    add_includedirs("include")
    add_includedirs("ut/include")

    --[[ taget main ]]
    add_files("ut/source/tests/executor_tests/inline_executor_tests.cpp")


target("manual_executor_tests")
    add_cxxflags("-fcoroutines -stdlib=libc++  ", {tools = "clang++"})
    set_kind("binary")
    add_deps("coroutine_lib")
    add_deps("ut_lib")
    add_includedirs("include")
    add_includedirs("ut/include")

    --[[ taget main ]]
    add_files("ut/source/tests/executor_tests/manual_executor_tests.cpp")
