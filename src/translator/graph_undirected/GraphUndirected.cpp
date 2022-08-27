#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "screen/Screen.h"
#include "translator/Translator.h"

// With emscripten, you may need to execute in the src directory.
// # ln -s /usr/include/boost/ boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_connected.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/properties.hpp>
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

std::string GraphUndirected::Translate(
    const std::string& input,
                                   const std::string& options_string) {
    
    auto options = SerializeOption(options_string);
   
    return "";
}
std::unique_ptr<Translator> GraphUndirectedTranslator() {
  return std::make_unique<GraphUndirected>();
}
