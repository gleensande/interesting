#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

template <typename T>
class Matrix {
 private:
    T** _arr;
    int _rows;
    int _cols;
 public:
    // методы, относящиеся к методу Гаусса
    void make_diag_one_Gauss(int i_from, Matrix<double>* fr);
    void substract_rows_Gauss(int i_from, int i_to, T coeff, Matrix<double>* fr);
    void forward_Gauss(Matrix<double>* fr);
    void backward_Gauss(Matrix<double>* fr);
    void Gauss(Matrix<double>* fr);

    // основные используемые методы для матриц
    Matrix(const int rows = 1, const int cols = 1);
    const int get_rows() const;
    const int get_cols() const;
    const T get(int i, int j) const;
    void set(int i, int j, T val);
    const bool check_i_j(int i, int j) const;
    void swap_rows(int i1, int i2);

    void print(Matrix<double>* fr, string message = "") const;
    ~Matrix();
};

#endif // MATRIX_HPP