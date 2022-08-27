find_package(Boost COMPONENTS graph)

if (Boost_FOUND OR EMSCRIPTEN)
  add_library(translator_graph_undirected STATIC
    GraphUndirected.cpp
  )

  target_link_libraries(translator_graph_undirected
    PRIVATE diagon_base
  )

  if (Boost_FOUND)
    target_link_libraries(translator_graph_undirected PRIVATE Boost::graph)
  else()
    message(WARNING "GraphUndirected: Boost::graph not found")
  endif()
else()
  add_library(translator_graph_undirected STATIC
    GraphUndirectedEmpty.cpp
  )
endif()

if (NOT MSVC)
  target_compile_options(translator_graph_undirected PRIVATE "-Wno-attributes")
endif()

target_set_common(translator_graph_undirected)
