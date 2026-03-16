#include "graphListAdj.hpp"

#include <algorithm>



bool GraphAdjVector::is_edge(vertex v1, vertex v2) const
{
    return std::find(_neighbors_set[v1].begin(), _neighbors_set[v1].end(), v2) !=
           _neighbors_set[v1].end();
}

void GraphAdjVector::add_edge(vertex v1, vertex v2)
{
    _neighbors_set[v1].push_back(v2);
    _neighbors_set[v2].push_back(v1);
    _nb_edges ++;
}
/*
void GraphAdjVector::delete_edge(vertex v1, vertex v2)
{
    if (delete_edge_aux(v1, v2)){
        delete_edge_aux(v2, v1);
        _nb_edges --;
    }
}
*/

gint GraphAdjVector::degree(vertex v) const{
    return _neighbors_set[v].size();
}

std::vector<vertex> GraphAdjVector::neighbors(vertex v) const
{
    std::vector<vertex> v2(_neighbors_set[v]);
    std::sort(v2.begin(), v2.end());
    return v2;
}


void GraphAdjVector::get_neighbors(vertex v1, vector<vertex> & sol) const
{
    sol=_neighbors_set[v1];

}
/*
bool GraphAdjVector::delete_edge_aux(vertex v1, vertex v2)
{
    for (auto it = _neighbors_set[v1].begin() ; it != _neighbors_set[v1].end(); ++it ){
        if (*it==v2){
            *it = _neighbors_set[v1].back();
            _neighbors_set[v1].pop_back();
            return true;
        }
    }
    return false;
}
*/
void GraphAdjVector::reset(vertex n)
{    _nb_edges = 0;
    _nb_vertices=n;
    _neighbors_set.clear();
    _neighbors_set.resize(n);
    _neighbors_set.shrink_to_fit();
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

/*
void GraphAdjVector::intersect_neighbors(vector<vertex>& vect, vertex v1) const{
    gint size_inter(0);
    //TODO
    for (auto v:vect){
        if (is_edge(v,v1)) vect[size_inter]=v;
        size_inter ++;
    }
    vect.resize(size_inter+1);
}

void GraphAdjVector::intersect_neighbors(vector<bool>& vect, vertex v1) const{
    //TODO
    for (auto v:_neighbors_set[v1]){
        if (vect[v] && !is_edge(v,v1)) vect[v]=false;
    }


}

void GraphAdjVector::intersect_neighbors(set<vertex>& s, vertex v1) const{
    //TODO
}

*/


void GraphAdjVectorSorted::intersect_neighbors(vector<vertex> &vect, vertex v1) const
{
    std::vector<vertex> neigh_v1 (neighbors(v1));
    std::vector<vertex> result = vect;
    std::set_intersection(result.begin(), result.end(), neigh_v1.begin(), neigh_v1.end(),
                          std::back_inserter(vect));
    //vect = std::move(result);
}


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
/*
void GraphHeavy::add_edge(vertex v1, vertex v2)
{
    GraphAdjVector::add_edge( v1, v2);
     _adjMat->add_edge(v1, v2);
}
*/
