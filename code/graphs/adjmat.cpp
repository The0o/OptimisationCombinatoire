#include "adjmat.hpp"
#include <iostream>
#include <fstream>


gint AdjMat::degree(vertex v){
    gint r=0;
    for(gint i=0 ; i< v ; i++) if (is_edge(i,v)) r++;
    for(gint i=v+1 ; i< nb_vertex() ; i++) if (is_edge(i,v)) r++;
    return r;
}

void AdjMat::display_screen(){

    gint n=nb_vertex();

    std::cout << "print adjacency matrix of size : " << n << std::endl;
    //std::cout << "size in memory: " << size_of() << std::endl << std::endl;

    for(gint i=0 ; i< n ; i++){
                for(gint j=0 ; j< n ; j++){
                    if (is_edge(i,j)) std::cout << "1 ";
                    else std::cout << "0 ";
                }
                std::cout << std::endl;
    }
    std::cout << std::endl;
}


AdjRect::AdjRect(gint n):_n(n), _matrix(n) {
    _matrix.shrink_to_fit();
    for (gint i=0; i< _n ; i++){
        _matrix[i].resize(n);
        _matrix[i].shrink_to_fit();
    }

};

/*
unsigned int AdjRect::size_of(){
    unsigned int siz = sizeof(_n) ; // + sizeof(_matrix) ;
    for(gint i=0; i< _n ; i++) siz += sizeof(_matrix[i]);
    return siz;
}
*/

void AdjSymRectDeg::add_edge(vertex r, vertex c){
    if (is_edge(r,c)) return;
    AdjSymRect::add_edge(r,c);
    _degrees[r]++;
    _degrees[c]++;

};

void AdjSymRectDeg::delete_edge(vertex r, vertex c){
    if (!is_edge(r,c)) return;
    AdjSymRect::delete_edge(r,c);
    _degrees[r]--;
    _degrees[c]--;

}

void AdjSymRectDeg::display_screen()
{
    AdjSymRect::display_screen();
    std::cout << "print degrees : "  << std::endl;
    for (auto i = _degrees.begin(); i != _degrees.end(); ++i)
        std::cout << *i << " ";
    std::cout << std::endl;
};


gint AdjSymRectDeg::degree(vertex v){
  return _degrees[v];
}
