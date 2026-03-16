#include "graphAdjMatrix.hpp"

gint GraphAdjMatrix::nb_vertices() const
{
    return _adjMat->nb_vertex();
}

gint GraphAdjMatrix::nb_edges() const
{
    gint cpt = 0;
    for (gint i = 0; i < nb_vertices() ; i++ ) cpt += degree(i);
    return cpt/2;
}

bool GraphAdjMatrix::is_edge(vertex v1, vertex v2) const
{
    return _adjMat->is_edge(v1, v2);
}

void GraphAdjMatrix::add_edge(vertex v1, vertex v2)
{
        _adjMat->add_edge(v1, v2);
}
/*
void GraphAdjMatrix::delete_edge(vertex v1, vertex v2)
{
    _adjMat->delete_edge(v1, v2);
}*/

gint GraphAdjMatrix::degree(vertex v) const
{
    return _adjMat->degree(v);
}

std::vector<vertex> GraphAdjMatrix::neighbors(vertex v1) const
{
    std::vector<vertex> vect;
    vect.reserve(nb_vertices());
    for (gint i = 0; i < nb_vertices() ; i++ )
        if (is_edge(v1,i)) vect.push_back(i);
    vect.shrink_to_fit();
    return vect;
}

void GraphAdjMatrix::get_neighbors(vertex v1, vector<vertex> & vect) const{
    vect.clear();
    vect.reserve(nb_vertices());
    for (gint i = 0; i < nb_vertices() ; i++ )
        if (is_edge(v1,i)) vect.push_back(i);
    vect.shrink_to_fit();
}


void GraphAdjMatrix::reset(vertex nb)
{
    switch(_type){

        case AdjMatType::AdjSymRect:
            _adjMat = std::make_unique<AdjSymRect>(nb);
                    break;

        case AdjMatType::AdjSymTrig:
            _adjMat = std::make_unique<AdjSymTrig>(nb);
            break;

        case AdjMatType::AdjSymRectDeg:
            _adjMat = std::make_unique<AdjSymRectDeg>(nb);
            break;
    }

}


