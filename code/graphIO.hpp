//#include <algorithm>
#include <fstream>
#include <iostream>
//#include <string>
#include "graph.hpp"
//#include "graphDefs.hpp"
//#include <sstream>

#include <vector>


void print_vector(vector<vertex> vec){
    for (auto v : vec) {
        cout << " " << v;
    }
    cout << endl;
}

/*
void to_dot_colored(const Graph &g,const std::string &filename,
                           const std::vector<color> &coloring)  {
    std::ofstream file(filename);
    file << "graph G {\n";
    // Style global
    file << "  node [style=filled, shape=circle];\n";
    // 1) Déclarer chaque nœud avec sa couleur et son label 1-based
    static const char* palette[] = {
        "white","red","green","blue","yellow","orange","cyan","magenta", "purple", "salmon", "pink", "brown","lightblue","lightgreen","violet","lightgray","darkgray","darkblue",
    };
    for (vertex v = 0; v < g.nb_vertices(); ++v) {
        int c = coloring[v];
        const char* col = (c > 0 && c < int(std::size(palette)))
                          ? palette[c]
                          : "gray";
        // On passe à la numérotation 1-based pour le label et l'ID du nœud
        file << "  " << (v)
             << " [label=\"" << (v)
             << "\", fillcolor=\"" << col
             << "\"];\n";
    }
    // 2) Déclarer les arêtes en 1-based également
    for (vertex u = 0; u < g.nb_vertices(); ++u) {
        for (vertex v = u + 1; v < g.nb_vertices(); ++v) {
            if (g.is_edge(u, v)) {
                file << "  " << (u)
                     << " -- " << (v)
                     << ";\n";
            }
        }
    }
    file << "}\n";
}
*/
void display(const Graph &g, std::ostream & f) {
    f << "DISPLAY GRAPH" << std::endl;
    f << "nb of vertices " << g.nb_vertices() << std::endl;
    f << "nb of edges " << g.nb_edges() << std::endl;
    for (vertex v1 = 0; v1 < g.nb_vertices(); ++v1) {
        f << "neighbors of " << v1 << " : " ;
        for (auto v2 : g.neighbors(v1)) {
            f << v2 << " ";
        }
        f << std::endl;
    }
        f << "END DISPLAY GRAPH" << std::endl;
}


void display_screen(const Graph &g) {
    display(g,std::cout);
}


void export_as_dimacs_file(const Graph & g,  const std::string &filename) {
    std::ofstream file(filename);
    file << "p edge " << g.nb_vertices() << " " << g.nb_edges() << std::endl;
    for (vertex v1 = 0; v1 < g.nb_vertices(); ++v1) {
        for (vertex v2 = v1; v2 < g.nb_vertices(); ++v2) {
            if (g.is_edge(v1, v2)) {
                file << "e " << v1+1 << " " << v2+1 << std::endl;
            }
        }
    }
}


// Convert the graph to a dot file.
void to_dot(const Graph & g, const std::string &filename)  {
    std::ofstream file(filename);
    file << "graph {" << std::endl;
    for (vertex vertex = 0; vertex < g.nb_vertices(); ++vertex) {
        file << vertex << ";" << std::endl;
    }
    for (vertex v1 = 0; v1 < g.nb_vertices(); ++v1) {
        for (vertex v2 = v1; v2 < g.nb_vertices(); ++v2) {
            if (g.is_edge(v1, v2)) {
                file << v1 << " -- " << v2 << ";" << std::endl;
            }
        }
    }
    file << "}" << std::endl;
}

