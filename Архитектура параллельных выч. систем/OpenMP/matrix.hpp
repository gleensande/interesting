#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
class Matrix {
 protected:
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
    void swap_rows(int i1, int i2);
    

    void Gauss_Jordan(Matrix<double>* fr);

    // конструкторы
    Matrix(const int rows = 1, const int cols = 1, const T value = 0);
    Matrix(const Matrix& m);
    Matrix<T>& operator= (const Matrix& m);

    // основные используемые методы для матриц
    int get_rows() const;
    int get_cols() const;
    T get(int i, int j) const;
    void set(int i, int j, T val);
    bool check_i(int i) const;
    bool check_j(int i) const;
    Matrix<T> get_block(int rows, int cols, int i_lu, int j_lu) const;
    void set_block(Matrix<T> block, int i_lu, int j_lu);

    // операции над матрицами
    void operator+=(Matrix<T> m);
    void operator-=(Matrix<T> m);
    void operator*=(double n);
    Matrix<T> operator*(double n);
    Matrix<T> operator-(Matrix<T> m);
    Matrix<T> operator+(Matrix<T> m);
    Matrix<T> operator/(double n);

    // печать
    void print(const string& message) const;
    void print_SLAU(Matrix<double>* fr = NULL, string message = "") const;
    int max_width_in_col(int j) const;
    void nice_print(string message = "") const;

    // методы для векторов
    double inf_norm();
    Matrix(map<string, double> m);
    map<string, T> to_map(vector<string> keys) const;

    ~Matrix();
};


#endif // MATRIX_HPP
