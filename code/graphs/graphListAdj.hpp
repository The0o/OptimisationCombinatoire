#pragma once

#include "../graph.hpp"

class GraphAdjVectorSorted : public Graph {

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
    GraphAdjVectorSorted(std::string &filename){import_dimacs_file(filename);};
    GraphAdjVectorSorted(gint nb_vertices, float proba_edges, gint seed){generate_random(nb_vertices, proba_edges, seed);};

    virtual ~GraphAdjVectorSorted() = default;

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
    virtual void intersect_neighbors(vector<vertex> &vect, vertex v1) const override;
    void intersect_neighbors(vector<bool>& vect, vertex v1) const override;
    void intersect_neighbors(set<vertex>& s, vertex v1) const override;


    //Union in place of  sorted vector vect with neighbors of v1
    void union_neighbors(vector<vertex>& vect, vertex v1) const override;
    void union_neighbors(vector<bool>& vect, vertex v1) const override;
    void union_neighbors(set<vertex>& s, vertex v1) const override;

    //Remove in place from  sorted vector vect the neighbors of v1
    void diff_neighbors(vector<vertex>& vect, vertex v1) const override;
    void diff_neighbors(vector<bool>& vect, vertex v1) const override;
    void diff_neighbors(set<vertex>& s, vertex v1) const override;

    virtual Graph * clone(bool complementary = false) const override;

  protected:
    // Reset the graph to a new size nb_vertices.
    virtual void reset(vertex nb_vertices) override;
};


