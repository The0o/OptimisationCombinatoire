#pragma once

#include <string>
#include <vector>
#include <set>
#include <climits>
#include "graphDefs.hpp"


//using namespace std;

using std::vector;
//using std::unique_ptr;
using std::set;


class Graph {
  public:
    virtual ~Graph() = default;

    // Returns the number of vertices in the graph.
    virtual gint nb_vertices() const = 0;

    // Returns the number of edges in the graph.
    virtual gint nb_edges() const = 0;

    // Whether the vertices v1 and v2 are connected by an edge
    virtual bool is_edge(vertex v1, vertex v2) const = 0;

    // Returns the degree of the vertex.
    // The degree of a vertex is the number of edges incident to it.
    virtual gint degree(vertex vertex) const = 0;

    // Returns the list of neighbors of the vertex v1.
    virtual vector<vertex> neighbors(vertex v1) const = 0;
    virtual void get_neighbors(vertex v1, vector<vertex> & sol) const = 0;

    //Intersect in place sorted vector vect with neighbors of v1
    virtual void intersect_neighbors(vector<vertex>& vect, vertex v1) const = 0;
    virtual void intersect_neighbors(vector<bool>& vect, vertex v1) const = 0;
    virtual void intersect_neighbors(set<vertex>& s, vertex v1) const = 0;

    //Union in place of  sorted vector vect with neighbors of v1
    virtual void union_neighbors(vector<vertex>& vect, vertex v1) const = 0;
    virtual void union_neighbors(vector<bool>& vect, vertex v1) const = 0;
    virtual void union_neighbors(set<vertex>& s, vertex v1) const = 0;

    //Remove in place from  sorted vector vect the neighbors of v1
    virtual void diff_neighbors(vector<vertex>& vect, vertex v1) const = 0;
    virtual void diff_neighbors(vector<bool>& vect, vertex v1) const = 0;
    virtual void diff_neighbors(set<vertex>& s, vertex v1) const = 0;

    // Import a graph from a file in the DIMACS format.


    //virtual vector<vertex> getVertices() const;


    virtual Graph * clone(bool complementary = false) const =0;


    //virtual std::unique_ptr<Graph> induced(const vector<vertex>& A,const vector<vertex>& B) const;

    // Add an edge between the vertices v1 and v2.
    virtual void add_edge(vertex v1, vertex v2) = 0;

  protected:

    virtual void import_dimacs_file(const std::string &name);//, bool reverse = false, unsigned int vertices_limit = UINT_MAX);

    virtual void charge(const std::string &filename);


    // Reset the graph to a new size nb_vertices.
    virtual void reset(gint nb_vertices) = 0;



    // Remove the edge between the vertices v1 and v2.
    //virtual void delete_edge(vertex v1, vertex v2) = 0;

    // Generate a random graph with nb_vertices vertices and a density of
    // proba_density [0,100].
    void generate_random(gint nb_vertices,
                         float proba_edges,
                         gint seed);
    
};

std::string getGraphPathInstance();
