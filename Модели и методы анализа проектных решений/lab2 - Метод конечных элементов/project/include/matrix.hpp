#ifndef MATRIX_H
#define MATRIX_H

#include "std_include.hpp"

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

    // конструкторы
    Matrix(const int rows = 1, const int cols = 1);

    // основные используемые методы для матриц
    int get_rows() const;
    int get_cols() const;
    T get(int i, int j) const;
    void set(int i, int j, T val);
    bool check_i_j(int i, int j) const;
    void swap_rows(int i1, int i2);

    // операции над матрицами
    void mul_num(double n);

    // печать
    void print(Matrix<double>* fr = NULL, string message = "") const;
    int max_width_in_col(int j) const;
    void nice_print(string message = "") const;

    ~Matrix();
};

#endif // MATRIX_H