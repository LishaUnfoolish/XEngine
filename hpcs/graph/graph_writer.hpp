
/***************************
@Author: Xhosa-LEE
@Contact: lixiaoxmm@163.com
@Time: 2023/02/29
***************************/
#pragma once
// base llvm
#include <fstream>
#include <iterator>
#include <span>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "hpcs/common/log.hpp"
#include "hpcs/common/macros.hpp"
#include "hpcs/graph/breadth_first_iterator.hpp"
#include "hpcs/graph/graph_traits.hpp"
#include "hpcs/traits/pointer_traits.hpp"
namespace XEngine {

/* Name traits */
HAS_MEMBER_TRAITS(Name);
template <typename T>
inline constexpr bool CheckSmartPointerHasName = requires {
  requires CheckSmartPointer<T>;
  requires HasMemberName<typename IsSmartPointer<T>::valueType>;
};
template <typename T>
inline constexpr bool CheckPointerHasName = requires {
  requires std::is_pointer_v<T>;
  requires HasMemberName<typename std::remove_pointer<T>::type>;
};

struct DefaultDOTGraphTraits {
  explicit DefaultDOTGraphTraits() = default;
  template <typename GraphType>
  [[nodiscard]] static constexpr std::string_view getGraphName(
      const GraphType& /*unused*/) noexcept {
    return "xEngine Graph";
  }
};

template <typename Ty>
struct DOTGraphTraits : public DefaultDOTGraphTraits {
  constexpr DOTGraphTraits() noexcept(
      std::is_nothrow_default_constructible_v<Ty>)
      : DefaultDOTGraphTraits() {}
};

template <typename GraphType>
class GraphWriter {
  using DOTTraits = DOTGraphTraits<GraphType>;
  using GTraits = GraphTraits<GraphType>;
  using NodeType = typename GTraits::NodeType;

 public:
  constexpr GraphWriter(std::ostream& output, const GraphType& graph) noexcept(
      std::is_nothrow_default_constructible_v<GraphType>)
      : output_(output), graph_(graph) {
    dot_traits_ = DOTTraits();
  }

  constexpr void WriteGraph(const std::string& title = "") noexcept {
    // Output the header for the graph...
    WriteHeader(title);
    // Emit all of the nodes in the graph...
    WriteNodes();
    // Emit all of the edges in the graph...
    WriteEdges();
    // Output the end of the graph
    WriteFooter();
  }

  constexpr void WriteHeader(const std::string& title) noexcept {
    std::string graph_name(dot_traits_.getGraphName(graph_));
    if (!title.empty()) {
      output_ << "digraph \"" << title << "\" {\n";
    } else if (!graph_name.empty()) {
      output_ << "digraph \"" << graph_name << "\" {\n";
    } else {
      output_ << "digraph unnamed {\n";
    }
    output_
        << "compound=true\n"
        << "graph [style=\"filled,rounded\",color=lightgray]\n"
        << "node "
           "[shape=record,style=filled,fillcolor=white,width=0.1,height=0.3]\n"
        << "edge [arrowsize=0.5]\n";
  }

  constexpr void WriteFooter() noexcept {
    // Finish off the graph
    output_ << "\n}\n";
  }

  constexpr void WriteNodes() noexcept {
    BfIterator<const GraphType> iter(graph_, 0);
    for (; !iter.IsEnd(); ++iter) {
      output_ << *iter << "[";
      if constexpr (HasMemberName<NodeType>) {
        output_ << "label=\"" << graph_.GetNode(*iter).Name();
      } else if constexpr (CheckSmartPointerHasName<NodeType> ||
                           CheckPointerHasName<NodeType>) {
        output_ << "label=\"" << graph_.GetNode(*iter)->Name();
      } else {
        output_ << "label=\"" << std::to_string(*iter);
      }
      output_ << "\"];\n";
    }
  }

  constexpr void WriteEdges() noexcept {
    BfIterator<const GraphType, true> iter(graph_, 0);
    for (; !iter.IsEnd(); ++iter) {
      output_ << iter.From() << "->" << *iter << ";\n";
    }
  }

  [[nodiscard]] constexpr std::ostream& GetOStream() noexcept {
    return output_;
  }

 private:
  std::ostream& output_;
  const GraphType& graph_;
  DOTTraits dot_traits_;
};

template <typename GraphType>
constexpr void WriteGraph(std::ostream& output, const GraphType& graph,
                          const std::string& title) noexcept {
  GraphWriter<GraphType> output_string(output, graph);
  output_string.WriteGraph(title);
}

template <typename GraphType>
constexpr void WriteGraph(const std::string& file_name, const GraphType& graph,
                          const std::string& title) noexcept {
  std::ostringstream output{};
  XEngine::WriteGraph(output, graph, title);

  if (!file_name.empty()) {
    std::ofstream file_out;
    file_out.open(file_name, std::ios::trunc);
    file_out << output.str();
  } else {
    XINFO << output.str();
  }
}

template <typename GraphType>
constexpr void ViewGraph(const GraphType& graph,
                         const std::string& file_name = "",
                         const std::string& title = "") noexcept {
  XEngine::WriteGraph(file_name, graph, title);
}

}  // namespace XEngine
