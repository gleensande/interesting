#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <new>
#include <vector>

using std::vector;
using std::string;

template <typename T>
class Matrix {
 protected:
    T** _arr;
    int _rows;
    int _cols;
 public:
    Matrix(const int rows = 1, const int cols = 1);
    Matrix(T* arr, int a, int b);
    int get_rows() const;
    int get_cols() const;
    T get(int i, int j) const;
    void set(int i, int j, T val);
    bool check_i(int i) const;
    bool check_j(int j) const;

    void print(const string& message, vector<int>* colors = nullptr ) const;
    ~Matrix();
};


#endif  // MATRIX_HPP
