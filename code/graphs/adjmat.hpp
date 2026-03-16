#pragma once

#include "../graphDefs.hpp"
#include <vector>

using namespace std;

class AdjMat {

public:
   virtual gint nb_vertex() = 0;
   virtual bool is_edge(vertex row, vertex col) = 0;
   virtual void add_edge(vertex row, vertex col) = 0;
   virtual void delete_edge(vertex row, vertex col) = 0;

   virtual void display_screen(); // peut être overridée
   //virtual unsigned int size_of() = 0;
   virtual gint degree(vertex v);

   virtual ~AdjMat() {}

};

// pour des graphes orientés

class AdjRect : public AdjMat{

private:
   vertex _n;
   vector<vector<bool>> _matrix;

public:
   AdjRect(gint n);
   gint nb_vertex(){return _n;};
   bool is_edge(vertex row, vertex col){ return _matrix[row][col];};
   void add_edge(vertex row, vertex col){_matrix[row][col]=true;};
   void delete_edge(vertex row, vertex col){_matrix[row][col]=false;};
   //unsigned int size_of();
};

class AdjRectFlat : public AdjMat{

private:
   vertex _n;
   vector<bool> _matrix;

public:
   AdjRectFlat(gint n):_n(n), _matrix(n*n) {_matrix.shrink_to_fit();};
   gint nb_vertex(){return _n;};
   bool is_edge(vertex row, vertex col){ return _matrix[_n * row+col];};
   void add_edge(vertex row, vertex col){_matrix[_n * row+col]=true;};
   void delete_edge(vertex row, vertex col){_matrix[_n * row+col]=false;};
   //unsigned int size_of(){return sizeof(_matrix) + sizeof(_n);};
};



// pour des graphes non orientés, matrice carrée symétrique
class AdjSymRect : public AdjMat{

private:
   vertex _n;
   vector<bool> _matrix;

public:
   AdjSymRect(vertex n):_n(n), _matrix(n*n) {_matrix.shrink_to_fit();};
   gint nb_vertex(){return _n;};
   bool is_edge(vertex r, vertex c){return _matrix[_n * r+c];};
   virtual void add_edge(vertex r, vertex c){_matrix[_n * r+c] =true; _matrix[_n * c+r] =true;};
   virtual void delete_edge(vertex r, vertex c){_matrix[_n * r+c] =false; _matrix[_n * c+r] =false;};
   //unsigned int size_of(){return sizeof(_matrix) + sizeof(_n);};
};

class AdjSymTrig : public AdjMat{

private:
   vertex _n;
   vector<bool> _matrix;

public:
   AdjSymTrig(vertex n):_n(n), _matrix(n*(n+1)/2) {_matrix.shrink_to_fit();};
   gint nb_vertex(){return _n;};
   bool is_edge(vertex r, vertex c){ vertex mi = std::min(r,c) ;  vertex ma = std::max(r,c);   return   _matrix[(ma*(ma+1))/2 +mi];};
   void add_edge(vertex r, vertex c){vertex mi = std::min(r,c) ;  vertex ma = std::max(r,c);   _matrix[(ma*(ma+1))/2 +mi] =true;};
   void delete_edge(vertex r, vertex c){vertex mi = std::min(r,c) ;  vertex ma = std::max(r,c);   _matrix[(ma*(ma+1))/2 +mi] =false;};
   //void display_screen() override;
   //unsigned int size_of(){return sizeof(_matrix) + sizeof(_n);};
};

// + stockage et mise à jour des degres
class AdjSymRectDeg : public AdjSymRect{

private:
   vector<gint> _degrees;

public:
   AdjSymRectDeg(vertex n):AdjSymRect(n), _degrees(n) {_degrees.shrink_to_fit();};
   void add_edge(vertex r, vertex c) override;
   void delete_edge(vertex r, vertex c) override;
   void display_screen() override;
   gint degree(vertex v) override;

};
