#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

using namespace std;

class Matrix {
 protected:
    double** _arr;
    int _rows;
    int _cols;
 public:
    // методы, относящиеся к методу Гаусса
    void make_diag_one_Gauss(int i_from, Matrix* fr);
    void substract_rows_Gauss(int i_from, int i_to, double coeff, Matrix* fr);
    void forward_Gauss(Matrix* fr);
    void backward_Gauss(Matrix* fr);
    void Gauss(Matrix* fr);

    Matrix(const int rows = 1, const int cols = 1, const double value = 0);

    ~Matrix();
};


#endif // MATRIX_HPP
