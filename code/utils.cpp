#include "utils.hpp"

#include <queue>





// Returns the number of vertices in the graph.
gint nb_edges(const Graph &g) {
    return g.nb_edges();
}

// Returns the density of the graph.
double density(const Graph &g) {
    return static_cast<double>(2.0 * g.nb_edges()) /
           (static_cast<double>(g.nb_vertices() * (g.nb_vertices() - 1)));
}

// Returns the maximum degree of the graph.
gint max_degree(const Graph &g) {
    gint max_degree = 0;
    for (vertex v = 0; v < g.nb_vertices(); ++v) {
        if (max_degree < g.degree(v)) max_degree =  g.degree(v);
    }
    return max_degree;
}


gint residual_degree(const Graph &g,const vector<vertex> & sub, vertex v){
    vector<vertex>  sub2(sub);
    g.intersect_neighbors(sub2, v);
    return sub2.size();
}


// Returns a pair of the max degree vertex and its residual degree.
std::pair<vertex, gint> max_residual_degree_vertex(const Graph &g,const vector<vertex> & sub) {
    gint max_degree = 0;
    gint max_degree_vertex = 0;
    for (vertex v : sub) {
        auto temp(residual_degree(g,sub,v));
        if (temp > max_degree) {
            max_degree = temp;
            max_degree_vertex = v;
        }
    }
    return std::make_pair(max_degree_vertex, max_degree);
}

// Returns a pair of the max degree vertex and its degree.
std::pair<gint, gint> max_degree_vertex(const Graph &g) {
    gint max_degree = 0;
    gint max_degree_vertex = 0;
    for (vertex v = 0; v < g.nb_vertices(); ++v) {
        if (g.degree(v) > max_degree) {
            max_degree = g.degree(v);
            max_degree_vertex = v;
        }
    }
    return std::make_pair(max_degree_vertex, max_degree);
}

gint max_degree_vertices(const Graph &g, std::vector<vertex> &vertices)
{
    gint max_degree = 0;
    vertices.clear();
    vertices.reserve(g.nb_vertices());
    for (vertex v = 0; v < g.nb_vertices(); ++v) {
        gint current_degree = g.degree(v);
        if (max_degree < current_degree) {
            max_degree =  current_degree;
            vertices.clear();
            vertices.push_back(v);
        }
        else if (current_degree == max_degree)
            vertices.push_back(v);
    }
    vertices.shrink_to_fit();
    return max_degree;
}



// Returns true if the vertices of the graph are a clique.
bool is_clique(const Graph &graph, const std::vector<vertex> &vertices) {
    for (gint i1 = 0 ;  i1 < vertices.size()-1; i1++) {
        vertex v1 = vertices[i1];
        for (gint i2 = i1+1 ;  i2 < vertices.size(); i2++) {
            if (!graph.is_edge(v1, vertices[i2])) {
                return false;
            }
        }
    }
    return true;
}

// Returns true if the vertices of the graph are a stable set.
bool is_stable_set(const Graph &graph, const std::vector<vertex> &vertices) {
    for (gint v1 : vertices) {
        for (gint v2 : vertices) {
            if (v1 != v2 && graph.is_edge(v1, v2)) {
                return false;
            }
        }
    };
    return true;
}




void breadth_first_search(const Graph &graph, const vertex start,  vector<vertex> & vertices_visited) {

    vertices_visited.clear();
    vertices_visited.reserve(graph.nb_vertices()) ;
    vector<bool> vertices_marked_bool;
    vertices_marked_bool.resize(graph.nb_vertices()) ;
    vertices_marked_bool[start]=true;
    queue<vertex> vertices_to_visit ;
    vertices_to_visit.push(start);

    while (!vertices_to_visit.empty()) {
        vertex current = vertices_to_visit.front();
        vertices_visited.push_back(current);
        vertices_to_visit.pop();
        for (auto v : graph.neighbors(current)) {
            if ( vertices_marked_bool[v]==false){
                vertices_to_visit.push(v) ;
                vertices_marked_bool[v]=true;
            }
        }
    }
    //return vertices_visited ;
}




void connected_components(const Graph &graph,  vector<vector<vertex>> & sol) {
    sol.clear();
    vector<bool> visited(graph.nb_vertices(),false);
    vector<vertex> vector_visited ;
    vector_visited.reserve(graph.nb_vertices()) ;

    for (vertex start = 0 ; start < graph.nb_vertices() ; start ++){
        if (!visited[start]){
            breadth_first_search(graph, start, vector_visited);
            sol.push_back(vector_visited);
            for (auto v : vector_visited) visited[v]=true;
            vector_visited.clear();
        }
    }
//    return sol;
}


void breadth_first_search(const Graph &graph, const vertex start, const vector<bool> & subset_vertices,  vector<vertex> & vertices_visited) {

    vertices_visited.clear();
    vertices_visited.reserve(graph.nb_vertices()) ;
    vector<bool> vertices_marked_bool;
    vertices_marked_bool.resize(graph.nb_vertices()) ;
    vertices_marked_bool[start]=true;
    queue<vertex> vertices_to_visit ;
    vertices_to_visit.push(start);

    while (!vertices_to_visit.empty()) {
        vertex current = vertices_to_visit.front();
        vertices_visited.push_back(current);
        vertices_to_visit.pop();
        for (auto v : graph.neighbors(current)) {
            if ( subset_vertices[v] && vertices_marked_bool[v]==false){
                vertices_to_visit.push(v) ;
                vertices_marked_bool[v]=true;
            }
        }
    }
    //return vertices_visited ;
}



void connected_components(const Graph &graph,  const vector<vertex> & subset_vertices,  vector<vector<vertex>> & sol) {
    sol.clear();
    vector<bool> visited(graph.nb_vertices(),false);
    vector<vertex> vector_visited ;
    vector_visited.reserve(graph.nb_vertices()) ;

    vector<bool>  subset(graph.nb_vertices(),false);
    for (auto v : subset_vertices) subset[v]=true;

    for (auto start : subset_vertices){
        if (!visited[start]){
            breadth_first_search(graph, start, subset, vector_visited);
            sol.push_back(vector_visited);
            for (auto v : vector_visited) visited[v]=true;
            vector_visited.clear();
        }
    }
//    return sol;
}

