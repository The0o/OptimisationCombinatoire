#pragma once
#include "../../graphs/graphDefs.hpp"
#include "../../graphs/graph.hpp"

using weight = gint;

weight getEdgeWeight  (const Graph & g, vertex v1, vertex v2);

weight getVertexWeight  (const Graph & g, vertex v);
