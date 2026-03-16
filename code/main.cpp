
#include "graphs/graphListAdj.hpp"
#include "graphs/graphAdjMatrix.hpp"

#include <iostream>
#include "utils.hpp"


void print_vector(vector<vertex> vec){
    for (auto v : vec) {
        cout << " " << v;
    }
    cout << endl;
}

int main(int argc , char* argv [])
{
    //string s("myciel");
    string s("C125.9.clq"); // nom de l'instance

    if (argc>=2) {
        s = argv[1];
        //s="../instances/"+s;
    }
    //GraphAdjMatrix g(s);
    //GraphHeavy g(s);
    //GraphAdjVectorSorted g(s);
    //GraphAdjVector g(s);
    //GraphAdjVector g(100,0.8,2);
    //GraphAdjMatrix g(100,0.8,2);
    GraphHeavy g(100,0.8,2);


// Returns the density of the graph.
   cout<<"Nombre de sommets du graphe : " << g.nb_vertices()<<endl;
   cout<<"Nombre d arretes du graphe : " << nb_edges(g)<<endl;
   cout<<"Densite du graphe : " << density(g)<<endl;
   cout<<"Degre max du graphe : " << max_degree(g)<<endl;
   cout<<"Parcours BFS du graphe partant de 0 : ";
   vector<vertex>  vertices_visit;
   breadth_first_search(g,0,vertices_visit);
   print_vector(vertices_visit);
   vector<vector<vertex>> comp;
    connected_components(g,   comp);
   for (auto a : comp) {
        cout<<"Composante connexe : ";
        print_vector(a);
       cout<<endl;
   }
   vector<vertex>  subset_vertices{0,1,2,55};
   print_vector(subset_vertices);
   connected_components(g, subset_vertices,  comp);
   for (auto a : comp) {
       cout<<"Composante connexe : ";
       print_vector(a);
       //cout<<endl;
   }
    return 0;
}
