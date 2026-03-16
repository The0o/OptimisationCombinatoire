#pragma once

#include "../graph.hpp"
#include "adjmat.hpp"
#include <memory>

class GraphAdjVector : public Graph {

  protected:
    // The number of vertices in the graph.
    gint _nb_vertices;

    // The number of edges in the graph.
    gint _nb_edges;

    // Represents the adjacency list of the graph.
    // The list size is nb_vertices.
    // Each element of the list is a list of vertices connected to the vertex.
    std::vector<std::vector<vertex>> _neighbors_set;

  public:
    GraphAdjVector(std::string &filename){import_dimacs_file(filename);};
    GraphAdjVector(gint nb_vertices, float proba_edges, gint seed){generate_random(nb_vertices, proba_edges, seed);};

    virtual ~GraphAdjVector() = default;

    // Returns the number of vertices in the graph.
    virtual gint nb_vertices() const override {return _nb_vertices;};

    // Returns the number of edges in the graph.
    virtual gint nb_edges() const override {return _nb_edges;};

    // Whether the vertices v1 and v2 are connected by an edge
    virtual bool is_edge(vertex v1, vertex v2) const override;

    // Add an edge between the vertices v1 and v2.
    virtual void add_edge(vertex v1, vertex v2) override;

    // Remove the edge between the vertices v1 and v2.
    //virtual void delete_edge(vertex v1, vertex v2) override;

    // Returns the degree of the vertex.
    // The degree of a vertex is the number of edges incident to it.
    virtual gint degree(vertex vertex) const override;

    // Returns the sorted list of neighbors of the vertex v1.
    virtual std::vector<vertex> neighbors(vertex v1) const override;
    virtual void get_neighbors(vertex v1, vector<vertex> & sol) const override;

    //Intersect in place sorted vector vect with neighbors of v1
    //virtual void intersect_neighbors(vector<vertex>& vect, vertex v1) const override;
    //virtual void intersect_neighbors(vector<bool>& vect, vertex v1) const override;
    //virtual void intersect_neighbors(set<vertex>& s, vertex v1) const override;

  protected:
    // Reset the graph to a new size nb_vertices.
    virtual void reset(vertex nb_vertices) override;
};

//considering sorted vectors, few operations to re-implement
// N.B: add_edge does not keep sortd vectors, managed at the end of constructors
class GraphAdjVectorSorted : public GraphAdjVector {

  public:
    GraphAdjVectorSorted(std::string &filename):GraphAdjVector(filename){};
    GraphAdjVectorSorted(gint nb_vertices, float proba_edges, gint seed):GraphAdjVector(nb_vertices, proba_edges,seed){};

    virtual ~GraphAdjVectorSorted() = default;


    // Whether the vertices v1 and v2 are connected by an edge: to re-implement
    bool is_edge(vertex v1, vertex v2) const override;

    void intersect_neighbors(vector<vertex> &vect, vertex v1) const override;

    // Returns the sorted list of neighbors of the vertex v1: to re-implement
    std::vector<vertex> neighbors(vertex v1) const override;

};

class GraphHeavy : public GraphAdjVectorSorted {
    private:
        std::unique_ptr<AdjMat> _adjMat;
        void compute_adjency_matrix();

    public:
        GraphHeavy(std::string &filename):GraphAdjVectorSorted(filename) {compute_adjency_matrix();};
        GraphHeavy(gint nb_vertices, float proba_edges, gint seed):GraphAdjVectorSorted(nb_vertices, proba_edges,seed){compute_adjency_matrix();};

        virtual ~GraphHeavy() = default;


        bool is_edge(vertex v1, vertex v2) const override;

        //void add_edge(vertex v1, vertex v2) override;

};
