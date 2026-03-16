#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <random>

//#include <cmath>
#include "graph.hpp"
#include "path.hpp"
//#include <sstream>

//#include "InstanceGraph.hpp"
//#include <unordered_map>
//#include <unordered_set>
//#include <vector>



using std::string;
//using std::min;


/*
void Graph::display_screen() const{
    display(std::cout);
}*/

// efficace quand on a une matrice d'adjacence
void Graph::intersect_neighbors(vector<vertex> &vect, vertex v1) const
{
    /*
    std::vector<vertex> neigh_v1 = neighbors(v1);
    std::vector<vertex> result;
    std::set_intersection(vect.begin(), vect.end(), neigh_v1.begin(), neigh_v1.end(),
                         std::back_inserter(result));
    vect = std::move(result);
*/
    //vector<vertex> copie = vect;
    //vector<vertex> neigh_v1 = neighbors(v1);
    gint size_inter(0);

    for (auto v:vect){
        if (is_edge(v,v1)) vect[size_inter]=v;
        size_inter ++;
    }
    vect.resize(size_inter+1);
}

// efficace quand on a une matrice d'adjacence
void Graph::intersect_neighbors(vector<bool> &vect, vertex v1) const
{  
    for (auto v(0); v < nb_vertices();v++){
        if (vect[v] && !is_edge(v,v1)) vect[v]=false;
    }

}

// efficace quand on a une matrice d'adjacence
void Graph::intersect_neighbors(set<vertex> &s, vertex v1) const
{

    for (auto it = s.begin(); it != s.end();)
    {
        if (!is_edge(*it,v1))
            it = s.erase(it);
        else
            ++it;
    }
}


/*
void Graph::intersect_neighbors(vector<vertex> &vect, vertex v1) const //ancienne version n'importe quoi renvoie vect concaténé a l'intersection (ou a peu près) ce qui fesait que ca marchait uniquement comme ça c'est que intersection était en paramètre de 
{
   std::vector<vertex> neigh_v1 = neighbors(v1);
   std::set_intersection(vect.begin(), vect.end(), neigh_v1.begin(), neigh_v1.end(),
                            std::back_inserter(vect));
}
*/
void Graph::union_neighbors(vector<vertex> &vect, vertex v1) const
{
    /*
    std::vector<vertex> neigh_v1 = neighbors(v1);
    std::vector<vertex> result;
    std::set_union(vect.begin(), vect.end(), neigh_v1.begin(), neigh_v1.end(),
                  std::back_inserter(result));
    vect = std::move(result);
    */
    std::vector<vertex> neigh_v1(neighbors(v1));
    std::vector<vertex> copie(vect);
    std::set_union(copie.begin(), copie.end(), neigh_v1.begin(), neigh_v1.end(),
                   std::back_inserter(vect));
}

void Graph::union_neighbors(vector<bool> &vect, vertex v1) const
{
    vector<vertex> neigh_v1;
    get_neighbors(v1,neigh_v1);
    for (auto v:neigh_v1){
        if(!vect[v]) vect[v]=true;
    }
}


void Graph::union_neighbors(set<vertex> &s, vertex v1) const
{
    vector<vertex> neigh_v1;
    get_neighbors(v1,neigh_v1);
    for (auto v:neigh_v1) s.insert(v);
}

//TODO a reprendre
void Graph::diff_neighbors(vector<vertex> &vect, vertex v1) const
{
    gint size_diff(0);

    vector<vertex> neigh_v1;
    get_neighbors(v1,neigh_v1);
    for (auto v:neigh_v1)
    {
        vect[size_diff]=v;
        size_diff ++;
    }
    vect.resize(size_diff+1);
}

void Graph::diff_neighbors(vector<bool> &vect, vertex v1) const
{
    vector<vertex> neigh_v1;
    get_neighbors(v1,neigh_v1);
    for (auto v:neigh_v1){
        if (vect[v]) vect[v]=false;
    }
}

void Graph::diff_neighbors(set<vertex> &s, vertex v1) const
{
    vector<vertex> neigh_v1;
    get_neighbors(v1,neigh_v1);
    for (auto v:neigh_v1) s.erase(v);
}



// Generate a random graph with nb_vertices vertices and a density of
// proba_density [0,100].
void Graph::generate_random(vertex nb_vertices,
                            float proba_edges,
                            vertex seed) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<vertex> distribution(0, 100);

    gint proba = static_cast<gint>(100 * proba_edges);
    if (proba > 100) proba = 100;

    reset(nb_vertices);
    for (vertex v1 = 0; v1 < nb_vertices; ++v1) {
        for (vertex v2 = v1 + 1; v2 < nb_vertices; ++v2) {
            if (distribution(generator) < proba) {
                add_edge(v1, v2);
            }
        }
    }
}


// Import a graph from a file in the DIMACS format.
void Graph::import_dimacs_file(const std::string &name) {
    const std::string file_name = string(path_instance) + string("/") + string(name);

    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file " + file_name);
    }

    vertex nb_vertices;
    vertex nb_edges;
    vertex vertex1;
    vertex vertex2;

    std::string first;
    file >> first;
    while (!file.eof()) {
        if (first == "e") {
            file >> vertex1 >> vertex2;
            vertex1--;
            vertex2--;
            add_edge(vertex1, vertex2);
        } else if (first == "c") {
            getline(file, first);
        } else if (first == "p") {
            file >> first >> nb_vertices >> nb_edges;
            reset(nb_vertices);
        } else {
            throw std::runtime_error("Unknown character while reading file: " + first);
        }
        file >> first;
    }
}

void Graph::charge(const std::string &filename)
{
    import_dimacs_file(filename);
}

std::string getGraphPathInstance()
{
    return path_instance;
}
