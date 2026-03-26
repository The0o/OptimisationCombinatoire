#include "graphListAdj.hpp"

#include <algorithm>


void GraphAdjVectorSorted::intersect_neighbors(vector<vertex> &vect, vertex v1) const
{
    std::vector<vertex> result;
    std::set_intersection(vect.begin(), vect.end(), _neighbors_set[v1].begin(), _neighbors_set[v1].end(),
                          std::back_inserter(result));
    vect = std::move(result);
}

void GraphAdjVectorSorted::intersect_neighbors(vector<bool> &vect, vertex v1) const
{
    vector<vertex> vect2;
    vect2.reserve(_nb_vertices);
    for (auto v(0); v < _nb_vertices;v++) if (vect[v]) vect2.push_back(v);
    std::vector<vertex> result;
    std::set_difference(vect2.begin(), vect2.end(), _neighbors_set[v1].begin(), _neighbors_set[v1].end(),
                          std::back_inserter(result));

    for (auto v : result) vect[v]=false;

}


void GraphAdjVectorSorted::intersect_neighbors(set<vertex> &s, vertex v1) const
{

    for (auto it = s.begin(); it != s.end();)
    {
        if (!is_edge(*it,v1))
            it = s.erase(it);
        else
            ++it;
    }
}


void GraphAdjVectorSorted::union_neighbors(vector<vertex> &vect, vertex v1) const
{

    //std::vector<vertex> neigh_v1 = neighbors(v1);
    std::vector<vertex> result;
    std::set_union(vect.begin(), vect.end(), _neighbors_set[v1].begin(), _neighbors_set[v1].end(),
                  std::back_inserter(result));
    vect = std::move(result);

}

void GraphAdjVectorSorted::union_neighbors(vector<bool> &vect, vertex v1) const
{
    for (auto v:_neighbors_set[v1]){
        if(!vect[v]) vect[v]=true;
    }
}


void GraphAdjVectorSorted::union_neighbors(set<vertex> &s, vertex v1) const
{
    for (auto v:_neighbors_set[v1]) s.insert(v);
}

void GraphAdjVectorSorted::diff_neighbors(vector<vertex> &vect, vertex v1) const
{
    std::vector<vertex> result;
    std::set_difference(vect.begin(), vect.end(), _neighbors_set[v1].begin(), _neighbors_set[v1].end(),
                   std::back_inserter(result));
    vect = std::move(result);
}

void GraphAdjVectorSorted::diff_neighbors(vector<bool> &vect, vertex v1) const
{
    for (auto v:_neighbors_set[v1]){
        if (vect[v]) vect[v]=false;
    }
}

void GraphAdjVectorSorted::diff_neighbors(set<vertex> &s, vertex v1) const
{
    for (auto v:_neighbors_set[v1]) s.erase(v);
}

void GraphAdjVectorSorted::add_edge(vertex v1, vertex v2)
{
    _neighbors_set[v1].push_back(v2);
    _neighbors_set[v2].push_back(v1);
    _nb_edges ++;
}

gint GraphAdjVectorSorted::degree(vertex v) const{
    return _neighbors_set[v].size();
}


void GraphAdjVectorSorted::get_neighbors(vertex v1, vector<vertex> & sol) const
{
    sol=_neighbors_set[v1];

}

void GraphAdjVectorSorted::reset(vertex n)
{    _nb_edges = 0;
    _nb_vertices=n;
    _neighbors_set.clear();
    _neighbors_set.resize(n);
    _neighbors_set.shrink_to_fit();
}

Graph *GraphAdjVectorSorted::clone(bool complementary) const
{
    Graph * g(nullptr);
    g = new GraphAdjVectorSorted(_nb_vertices,0,1);
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



bool GraphAdjVectorSorted::is_edge(vertex v1, vertex v2) const
{
    //if (degree(v1)>degree(v2)) return is_edge(v2, v1);
    gint i_lb (0);
    gint i_ub (degree(v1)-1);
    gint i_mid ((i_lb+i_ub)/2);
    gint mid;
    while(i_lb+1 < i_ub){
        mid = _neighbors_set[v1][i_mid];
        i_mid=(i_lb+i_ub)/2;
        if (mid == v2) return true;
        if (mid < v2){
            i_lb = i_mid;
        }
        else {
            i_ub = i_mid;
        }
    }
    return _neighbors_set[v1][i_lb]==v2 or _neighbors_set[v1][i_ub]==v2;

}

std::vector<vertex> GraphAdjVectorSorted::neighbors(vertex v1) const
{
    return _neighbors_set[v1];
}
