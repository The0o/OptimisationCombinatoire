#include "graphHeavy.hpp"

void GraphHeavy::compute_adjency_matrix()
{
    _adjMat = std::make_unique<AdjSymRect>(nb_vertices());

    for (vertex v(0); v< nb_vertices(); v++)
        for(auto v2: _neighbors_set[v]) if (v2>v) _adjMat->add_edge(v, v2);
}

bool GraphHeavy::is_edge(vertex v1, vertex v2) const
{
    return _adjMat->is_edge(v1, v2);
}

void GraphHeavy::add_edge(vertex v1, vertex v2)
{
    GraphAdjVectorSorted::add_edge( v1, v2);
    _adjMat->add_edge(v1, v2);
}



Graph *GraphHeavy::clone(bool complementary) const
{
    Graph * g(nullptr);
    g = new GraphHeavy(_nb_vertices,0,1);
    //g->reset(_nb_vertices);
    if(complementary){
        for (gint i1(0) ;  i1 < _nb_vertices-1; i1++) {
            for (gint i2(i1+1) ;  i2 < _nb_vertices; i2++) {
                if (is_edge(i1, i2)) g->add_edge(i1,i2);
            }
        }
    }
    else{
        for (gint i1(0) ;  i1 < _nb_vertices-1; i1++) {
            for (gint i2(i1+1) ;  i2 < _nb_vertices; i2++) {
                if (!is_edge(i1, i2)) g->add_edge(i1,i2);
            }
        }
    }
    return g;
}


void GraphHeavy::intersect_neighbors(vector<vertex> &vect, vertex v1) const
{
    gint size_inter(0);
/*
    for (auto i(0);i<vect.size();i++){
        if (is_edge(vect[i],v1)){
            vect[size_inter]=vect[i];
            size_inter ++;
        }
    }*/
    for (auto v:vect){
        if (is_edge(v,v1)){
            vect[size_inter]=v;
            size_inter ++;
        }
    }
    vect.resize(size_inter);
}

void GraphHeavy::intersect_neighbors(vector<bool> &vect, vertex v1) const
{
    for (auto v(0); v < nb_vertices();v++){
        if (vect[v] && !is_edge(v,v1)) vect[v]=false;
    }

}

void GraphHeavy::intersect_neighbors(set<vertex> &s, vertex v1) const
{

    for (auto it = s.begin(); it != s.end();)
    {
        if (!is_edge(*it,v1))
            it = s.erase(it);
        else
            ++it;
    }
}

void GraphHeavy::diff_neighbors(vector<vertex> &vect, vertex v1) const
{

    gint size_diff(0);
    for (auto v:vect)
    {
        if (!is_edge(v,v1)){
            vect[size_diff]=v;
            size_diff++;
        }
    }
    vect.resize(size_diff);

}
