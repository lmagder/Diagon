#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "screen/Screen.h"
#include "translator/Translator.h"
#include "translator/antlr_error_listener.h"
#include "translator/graph_undirected/GraphUndirectedLexer.h"
#include "translator/graph_undirected/GraphUndirectedParser.h"

// With emscripten, you may need to execute in the src directory.
// # ln -s /usr/include/boost/ boost
#include <boost/graph/edge_list.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

class GraphUndirected : public Translator {
 public:
  GraphUndirected() {}
  const char* Name() final { return "Graph - undirected"; }
  const char* Identifier() final { return "GraphUndirected"; }
  const char* Description() final {
    return "Build a graph from node and edges";
  }
  std::vector<Translator::OptionDescription> Options() final;
  std::vector<Translator::Example> Examples() final;
  std::string Translate(const std::string& input,
                        const std::string& options_string) final;

};

std::vector<Translator::OptionDescription> GraphUndirected::Options() {
  return {};
}

std::vector<Translator::Example> GraphUndirected::Examples() {
  return {
      {
          "if then else loop",
          "if -> \"then A\" -> end\n"
          "if -> \"then B\" -> end\n"
          "end -> loop -> if",
      },
      {
          "test",
          "A -- B\n"
          "A -- C\n"
          "A -- D -- G\n"
          "B -- Z\n"
          "C -- Z",
      },
  };
}

static std::string ReadNode(GraphUndirectedParser::NodeContext* node) {
  if (auto id = node->ID()) {
    return id->getSymbol()->getText();
  } else if (auto text = node->STRING()) {
    auto label = text->getSymbol()->getText();
    // Remove the two '"'.
    return label.substr(1, label.size() - 2);
  } else {
    return "";
  }
}

std::string GraphUndirected::Translate(const std::string& input,
                                   const std::string& options_string) {
  auto options = SerializeOption(options_string);
  antlr4::ANTLRInputStream input_stream(input);

  // Lexer.
  GraphUndirectedLexer lexer(&input_stream);
  antlr4::CommonTokenStream tokens(&lexer);
  tokens.fill();

  // Parser:
  AntlrErrorListener error_listener;
  GraphUndirectedParser parser(&tokens);
  parser.addErrorListener(&error_listener);
  GraphUndirectedParser::GraphContext* context = nullptr;
  try {
    context = parser.graph();
  } catch (...) {
    return "";
  }

  struct Node {
    std::string label;
    boost::square_topology<>::point pt;
  };

  boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Node>
      graph;
  std::map<std::string, decltype(graph)::vertex_descriptor> nodeIds;
  std::set<std::pair<decltype(graph)::vertex_descriptor,
                     decltype(graph)::vertex_descriptor>>
      inputEdges;
  for (auto parsedEdge : context->edges()) 
  {
    auto nodes = parsedEdge->node();
    std::vector<decltype(graph)::vertex_descriptor> parsedNodes;
    parsedNodes.reserve(nodes.size());
    for (auto ctx : nodes) {
      auto r = nodeIds.emplace(ReadNode(ctx), ~0U);
      if (r.second)
        r.first->second = boost::add_vertex(Node{r.first->first}, graph);

      parsedNodes.push_back(r.first->second);
    }
    for (size_t i = 1; i < parsedNodes.size(); i++) 
    {
      auto p = std::make_pair(parsedNodes[i - 1], parsedNodes[i]);
      if (p.first > p.second)
        std::swap(p.first, p.second);
      inputEdges.emplace(std::move(p));
    }
  }

  for (const auto& e : inputEdges)
    boost::add_edge(e.first, e.second, graph);

  auto rnd = boost::random::minstd_rand(42);
  boost::square_topology<> topo(rnd);

  boost::random_graph_layout(graph, boost::get(&Node::pt, graph), topo);
  boost::fruchterman_reingold_force_directed_layout(
      graph, boost::get(&Node::pt, graph), topo);
  return "";
}
std::unique_ptr<Translator> GraphUndirectedTranslator() {
  return std::make_unique<GraphUndirected>();
}
