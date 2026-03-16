#pragma once

#include "adjmat.hpp"
#include "../graph.hpp"
#include <memory>

enum class AdjMatType { 
    //AdjRect, // N.B: graphes orientes
    //AdjRectFlat // N.B: graphes orientes
    AdjSymRectDeg, 
    AdjSymRect, 
    AdjSymTrig 
};

class GraphAdjMatrix: public Graph {
    private:
        std::unique_ptr<AdjMat> _adjMat;
        AdjMatType _type;
    
    public:
        GraphAdjMatrix(std::string &filename, AdjMatType type = AdjMatType::AdjSymRectDeg):
             _type(type) {import_dimacs_file(filename);};
            
        //GraphAdjMatrix(std::string &filename, bool reverse, AdjMatType type = AdjMatType::AdjSymRectDeg):
        //     _type(type) {import_dimacs_file(filename,*this, reverse);};
        //GraphAdjMatrix(std::string &filename):
        //     _type(AdjMatType::AdjSymRectDeg) {import_dimacs_file(filename);};
        GraphAdjMatrix(vertex nb_vertices, float proba_edges, vertex seed): _type(AdjMatType::AdjSymRectDeg) {
            generate_random(nb_vertices, proba_edges, seed);
        };
        GraphAdjMatrix(GraphAdjMatrix const & s) =delete;

        ~GraphAdjMatrix() = default;

        // Returns the number of vertices in the graph.
        gint nb_vertices() const override;

        // Returns the number of edges in the graph.
        gint nb_edges() const override;

        // Whether the vertices v1 and v2 are connected by an edge
        bool is_edge(vertex v1, vertex v2) const override;

        // Add an edge between the vertices v1 and v2.
        void add_edge(vertex v1, vertex v2) override;

        // Remove the edge between the vertices v1 and v2.
        //virtual void delete_edge(vertex v1, vertex v2) override;

        // Returns the degree of the vertex.
        // The degree of a vertex is the number of edges incident to it.
        gint degree(vertex vertex) const override;

        // Returns the sorted list of neighbors of the vertex v1.
        std::vector<vertex> neighbors(vertex v1) const override;
        virtual void get_neighbors(vertex v1, vector<vertex> & sol) const override;

      protected:
        // Reset the graph to a new size nb_vertices.
        void reset(vertex nb_vertices) override;
};
