package(default_visibility = ["//visibility:public"])

cc_library(
    name = "adjacency_iterator",
    hdrs = [
        "adjacency_iterator.hpp",
    ],
    deps = [
        "//hpcs/common:macros",
        "//hpcs/graph:graph_traits",
    ],
)

cc_library(
    name = "adjacency_list",
    hdrs = [
        "adjacency_list.hpp",
    ],
    deps = [
    ],
)

cc_library(
    name = "adjacency_matrix",
    hdrs = [
        "adjacency_matrix.hpp",
    ],
    deps = [
        "//hpcs/common:macros",
        "//hpcs/core:signal",
    ],
)

cc_library(
    name = "breadth_first_iterator",
    hdrs = [
        "breadth_first_iterator.hpp",
    ],
    deps = [
        "//hpcs/graph:adjacency_iterator",
    ],
)

cc_library(
    name = "chained_forward_star",
    hdrs = [
        "chained_forward_star.hpp",
    ],
    deps = [
    ],
)

cc_library(
    name = "dense_graph",
    hdrs = [
        "dense_graph.hpp",
    ],
    deps = [
        "//hpcs/graph:adjacency_matrix",
        "//hpcs/graph:graph_traits",
        "//hpcs/graph:matrix",
    ],
)

cc_library(
    name = "graph_traits",
    hdrs = [
        "graph_traits.hpp",
    ],
    deps = [
        "//hpcs/graph:iterator_range",
    ],
)

cc_library(
    name = "graph_writer",
    hdrs = [
        "graph_writer.hpp",
    ],
    deps = [
        "//hpcs/common:log",
        "//hpcs/common:macros",
        "//hpcs/graph:breadth_first_iterator",
        "//hpcs/graph:graph_traits",
        "//hpcs/traits:pointer_traits",
    ],
)

cc_library(
    name = "graph",
    hdrs = [
        "graph.hpp",
    ],
    deps = [
        "//hpcs/graph:adjacency_iterator",
        "//hpcs/graph:graph_traits",
        "//hpcs/graph:graph_writer",
    ],
)

cc_library(
    name = "topology_sort",
    hdrs = [
        "topology_sort.hpp",
    ],
    deps = [
        "//hpcs/graph:adjacency_iterator",
    ],
)

cc_library(
    name = "iterator_range",
    hdrs = [
        "iterator_range.hpp",
    ],
)

cc_library(
    name = "matrix",
    hdrs = [
        "matrix.hpp",
    ],
    deps = [
        "//hpcs/common:macros",
        "//hpcs/graph:iterator_range",
    ],
)

cc_test(
    name = "test_graph",
    srcs = [
        "test_graph.cpp",
    ],
    deps = [
        "//hpcs/common:catch",
        "//hpcs/graph",
        "//hpcs/graph:dense_graph",
    ],
)

cc_test(
    name = "test_adjacency_list",
    srcs = [
        "test_adjacency_list.cpp",
    ],
    deps = [
        "//hpcs/common:catch",
        "//hpcs/graph:adjacency_list",
    ],
)

cc_test(
    name = "test_matrix",
    srcs = [
        "test_matrix.cpp",
    ],
    deps = [
        "//hpcs/common:catch",
        "//hpcs/common:macros",
        "//hpcs/graph:matrix",
    ],
)
