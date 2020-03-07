#include "matrix.hpp"

using std::cout;
using std::endl;

// Конструктор пустой матрицы по строкам и столбцам
template <typename T>
Matrix<T>::Matrix(const int rows, const int cols) {
    _rows = rows;
    _cols = cols;
    _arr = new T*[rows];
    for (int i = 0; i < rows; i++) {
        _arr[i] = new T[cols];
    }
}

// Конструктор матрицы по двумерному массиву и размерам
template <typename T>
Matrix<T>::Matrix(T* arr, int rows, int cols) {
    _rows = rows;
    _cols = cols;
    _arr = new T*[rows];
    for (int i = 0; i < rows; i++) {
        _arr[i] = new T[cols];
    }
    for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
	   		set(i, j, arr[i*cols + j]);
		}
	}
}

// Возвращает количество строк в матрице
template <typename T>
int Matrix<T>::get_rows() const {
    return _rows;
}

// Возвращает количество столбцов в матрице
template <typename T>
int Matrix<T>::get_cols() const {
    return _cols;
}

// Возвращает элемент с индексами i и j
template <typename T>
T Matrix<T>::get(int i, int j) const {
    if (!(check_i(i) && check_j(j))) {
        return static_cast<T> (0);
    }

    return _arr[i][j];
}

// Устанавливает элемент с индексами i и j в значение val
template <typename T>
void Matrix<T>::set(int i, int j, T val)  {
    if (!(check_i(i) && check_j(j))) {
        return;
    }

    _arr[i][j] = val;
}

// Проверяет строку на присутствие в матрице
template <typename T>
bool Matrix<T>::check_i(int i) const { 
    if (i >= _rows) {
        cout << "too high row:" << i << endl;
        return false;
    } else if (i < 0) {
        cout << "too low row:" << i << endl;
        return false;
    }

    return true;
}

// Проверяет столбец на присутствие в матрице
template <typename T>
bool Matrix<T>::check_j(int j) const { 
    if (j >= _cols) {
        cout << "too high col:" << j << endl;
        return false;
    } else if (j < 0) {
        cout << "too low col:" << j << endl;
        return false;
    }

    return true;
}

// Печатает переданное сообщение и саму матрицу
template <typename T>
void Matrix<T>::print(const string& message, vector<int>* colors) const {
    cout.precision(3);
    cout << message << endl;
    if (colors == nullptr) {
        for (int i = 0; i < _rows; i++) {
            cout << "| ";
            for (int j = 0; j < _cols; j++) {
                cout << " " << _arr[i][j] << "  ";
            }
            cout << "|" << endl;
        }
        cout << endl;
    } else {
        for (int i = 0; i < _rows; i++) {
            cout << "| ";
            for (int j = 0; j < _cols; j++) {
                if (j == colors->at(i) - 1) {
                    cout << "_" << _arr[i][j] << "_ ";
                } else {
                    cout << " " << _arr[i][j] << "  ";
                }
            }
            cout << "|" << endl;
        }
        cout << endl;
    }
}

// Удаляет матрицу и очищает память
template <typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < _rows; i++) {
        delete[] _arr[i];
    }
    delete[] _arr;
}

template class Matrix<int>;

