#include "matrix.hpp"

// конструктор матрицы по строкам и столбцам
template <typename T>
Matrix<T>::Matrix(const int rows, const int cols) {
    _rows = rows;
    _cols = cols;
    _arr = new T*[rows];
    for (int i = 0; i < rows; i++) {
        _arr[i] = new T[cols];
    }
}

// возвращает количество строк в матрице
template <typename T>
const int Matrix<T>::get_rows() const {
    return _rows;
}

// возвращает количество столбцов в матрице
template <typename T>
const int Matrix<T>::get_cols() const {
    return _cols;
}

// возвращает элемент с индексами i и j
template <typename T>
const T Matrix<T>::get(int i, int j) const {
    if (!check_i_j(i, j)) {
        return 0;
    }

    return _arr[i][j];
}

// устанавливает элемент с индексами i и j в значение val
template <typename T>
void Matrix<T>::set(int i, int j, T val)  {
    if (!check_i_j(i, j)) {
        return;
    }

    _arr[i][j] = val;
}

// проверяет на корректность номера элементов
template <typename T>
const bool Matrix<T>::check_i_j(int i, int j) const { 
    if (i >= _rows) {
        cout << "too high row:" << i << endl;
        return false;
    } else if (j >= _cols) {
        cout << "too high col:" << j << endl;
        return false;
    } else if (i < 0) {
        cout << "too low row:" << i << endl;
        return false;
    } else if (j < 0) {
        cout << "too low col:" << j << endl;
        return false;
    }

    return true;
}

// делит главную строку на диагональный элемент,
// чтобы там была единица
template <typename T>
void Matrix<T>::make_diag_one_Gauss(int i_from, Matrix<double>* fr) {
    T coeff = _arr[i_from][i_from];
    for (int j = 0; j < _cols; j++) {
        _arr[i_from][j] = _arr[i_from][j] / coeff;
    }
    fr->set(i_from, 0, (fr->get(i_from, 0) / coeff));

}

// вычитает из обрабатываемой строки главную,
// умноженную на рассчитанный коэффициент
template <typename T>
void Matrix<T>::substract_rows_Gauss(int i_from, int i_to, T coeff, Matrix<double>* fr) {
    for (int j = 0; j < _cols; j++) {
        _arr[i_to][j] -= _arr[i_from][j] * coeff;
    }
    fr->set(i_to, 0, fr->get(i_to, 0) - (fr->get(i_from, 0) * coeff));
}

// выполняет прямой ход метода Гаусса
template <typename T>
void Matrix<T>::forward_Gauss(Matrix<double>* fr) {
    //cout << "Forward: " << endl;
    int i;
    for (i = 0; i < get_rows() - 1; i++) {
        for (int k = i + 1; k < get_rows(); k++) {
            make_diag_one_Gauss(i, fr);
            T coeff = _arr[k][i];
            substract_rows_Gauss(i, k, coeff, fr);
        //    print(fr);
        }
    }
    make_diag_one_Gauss(i, fr);
    //print(fr);
}

// выполняет обратный ход метода Гаусса
template <typename T>
void Matrix<T>::backward_Gauss(Matrix<double>* fr) {
    //cout << "Backward: " << endl;
    int i;
    for (i = get_rows() - 1; i > 0 ; i--) {
        for (int k = i - 1; k >= 0; k--) {
            T coeff = _arr[k][i];
            substract_rows_Gauss(i, k, coeff, fr);
            //print(fr);
        }
    }
    //print(fr);
}

// выполняет приведение матрицы к диагональному виду методом Гаусса
template <typename T>
void Matrix<T>::Gauss(Matrix<double>* fr) {
    forward_Gauss(fr);
    backward_Gauss(fr);
}

// печатает переданное сообщение, саму матрицу 
// и матрицу свободных членов
template <typename T>
void Matrix<T>::print(Matrix<double>* fr, string message) const {
    cout.precision(3);
    cout << message << endl;
    for (int i = 0; i < _rows; i++) {
        cout << "| ";
        for (int j = 0; j < _cols; j++) {
            cout << setw(5) << _arr[i][j] << " ";
        }
        cout << "|";
        if (fr != NULL) {
            cout << "  |" << setw(5) << fr->get(i, 0) << "|" << endl;
        } else {
            cout << endl;
        }
    }
    cout << endl;
}

// меняет местами строки
template <typename T>
void Matrix<T>::swap_rows(int i1, int i2) {
    check_i_j(i1, 0);  // TODO: not good
    check_i_j(i2, 0);  // TODO: not good
    T sw;
    for (int j = 0; j < _cols; j++) {
        sw = _arr[i1][j];
        _arr[i1][j] = _arr[i2][j];
        _arr[i2][j] = sw;
    }
    return;
}

// удаляет матрицу и очищает память
template <typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < _rows; i++) {
        delete[] _arr[i];
    }
    delete[] _arr;
}

template class Matrix<double>;