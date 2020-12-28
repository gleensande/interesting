#include "matrix.hpp"


// ------------------------- метод Гаусса ------------------------- //

// делит главную строку на диагональный элемент,
// чтобы там была единица
void Matrix::make_diag_one_Gauss(int i_from, Matrix* fr) {
    double coeff = _arr[i_from][i_from];
    for (int j = 0; j < _cols; j++) {
        _arr[i_from][j] = _arr[i_from][j] / coeff;
    }
    fr->set(i_from, 0, (fr->get(i_from, 0) / coeff));
}

// вычитает из обрабатываемой строки главную,
// умноженную на рассчитанный коэффициент
void Matrix::substract_rows_Gauss(int i_from, int i_to, double coeff, Matrix* fr) {
    for (int j = 0; j < _cols; j++) {
        _arr[i_to][j] -= _arr[i_from][j] * coeff;
    }
    fr->set(i_to, 0, fr->get(i_to, 0) - (fr->get(i_from, 0) * coeff));
}

// выполняет приведение матрицы к диагональному виду методом Гаусса
void Matrix::Gauss(Matrix* fr) {
    // forward
    for (int i = 0; i < _rows - 1; i++) {
        for (int k = i + 1; k < _rows; k++) {
            make_diag_one_Gauss(i, fr);
            double coeff = _arr[k][i];
            substract_rows_Gauss(i, k, coeff, fr);
        }
    }
    make_diag_one_Gauss(_rows - 1, fr);

    // backward
    for (int i = _rows - 1; i > 0 ; i--) {
        for (int k = i - 1; k >= 0; k--) {
            double coeff = _arr[k][i];
            substract_rows_Gauss(i, k, coeff, fr);
        }
    }
}

// ------------------------- метод Гаусса ------------------------- //

// конструктор матрицы по строкам, столбцам и начальному значению ячеек
Matrix::Matrix(const int rows, const int cols, const double value) {
    _rows = rows;
    _cols = cols;
    _arr = new double*[rows];
    for (int i = 0; i < rows; i++) {
        _arr[i] = new double[cols];
    }
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] = value;
        }
    }
}

// Удаляет матрицу и очищает память
Matrix::~Matrix() {
    if(_arr)
    {
        for (int i = 0; i < _rows; i++) {
            delete[] _arr[i];
        }
    }
    delete[] _arr;
    _arr = nullptr;
}
