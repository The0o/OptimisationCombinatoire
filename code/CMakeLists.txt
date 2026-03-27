cmake_minimum_required(VERSION 3.21)
project(tp_graphes)

set(CMAKE_CXX_STANDARD 17)

# -Wsign-conversion -Weffc++
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_ARCH_FLAGS} -O0 -g -lm -isystem dir")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_ARCH_FLAGS} -O3 -DNDEBUG -lm")

add_executable(exemple
    main.cpp
    graph.cpp
    graphs/graphListAdj.cpp
    graphs/adjmat.cpp
    graphs/graphAdjMatrix.cpp
    graphs/graphHeavy.cpp
    mcp_experiment.cpp
    weightedgraphDefs.cpp
    utils.cpp
)

#set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
