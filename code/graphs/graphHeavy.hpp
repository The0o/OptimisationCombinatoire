#pragma once

#include "adjmat.hpp"
#include "graphListAdj.hpp"
#include <memory>

/*
enum class AdjMatType { 
    //AdjRect, // N.B: graphes orientes
    //AdjRectFlat // N.B: graphes orientes
    AdjSymRectDeg, 
    AdjSymRect, 
    AdjSymTrig 
};*/

class GraphHeavy : public GraphAdjVectorSorted {
private:
    std::unique_ptr<AdjMat> _adjMat;
    void compute_adjency_matrix();

public:
    GraphHeavy(std::string &filename):GraphAdjVectorSorted(filename) {compute_adjency_matrix();};
    GraphHeavy(gint nb_vertices, float proba_edges, gint seed):GraphAdjVectorSorted(nb_vertices, proba_edges,seed){compute_adjency_matrix();};

    virtual ~GraphHeavy() = default;


    bool is_edge(vertex v1, vertex v2) const override;

    virtual Graph * clone(bool complementary = false) const override;


    void add_edge(vertex v1, vertex v2) override;

    void intersect_neighbors(vector<vertex> &vect, vertex v1) const override;
    void intersect_neighbors(vector<bool> &vect, vertex v1) const override;
    void intersect_neighbors(set<vertex> &vect, vertex v1) const override;

    void diff_neighbors(vector<vertex> &vect, vertex v1) const override;

};
