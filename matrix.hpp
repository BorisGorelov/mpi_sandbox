#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
#include <assert.h>
#include <array>
#include "profile.hpp"

using namespace std;

template <class T>
class Data {
public:
  Data() = default;
  Data(int numrows, int numcols)
    :Nrow(numrows), Ncol(numcols), mat(Nrow*Ncol), vec(numrows) {}

  Data(int numrows, int numcols, T* data)
    :Nrow(numrows), Ncol(numcols), mat(data, data+numrows*numcols), vec(numrows) {}

  T operator() (int row, int col) const {return mat[Ncol*row + col];}
  T& operator() (int row, int col) {return mat[Ncol*row + col];}

  T* matrix_pointer() {return mat.data();}
  T* vector_pointer() {return vec.data();}
  vector<T>& get_vec() {return vec;}

  int rows() {return Nrow;}
  int cols() {return Ncol;}

  void FillRandom(int row, int col) {
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> distrib(-10, 10);  // distrib(gen)
    for (int row = 0; row < rows(); ++row) {
      for (int col = 0; col < cols(); ++col)
        this(row, col) = static_cast<T>(distrib(gen));
      vec[row] = static_cast<T>(distrib(gen));
    }
  }

  int& get_Nrow() {return Nrow;}
  int& get_Ncol() {return Ncol;}
  void ReadFromFile(fstream& f) {
    assert(f);
    f >> get_Nrow() >> get_Ncol();
    T buf;
    mat.resize(Nrow * Ncol);
    vec.resize(Nrow);
    for (int row = 0; row < rows(); ++row) {
      for (int col = 0; col < cols(); ++col) {
        f >> buf;
        mat[Ncol*row + col] = buf;
      }
    }
    for (int col = 0; col < cols(); ++col) {
      f >> buf;
      vec[col] = buf;
    }
  }

  int Nrow, Ncol;
  vector<T> mat;
  vector<T> vec;
};
