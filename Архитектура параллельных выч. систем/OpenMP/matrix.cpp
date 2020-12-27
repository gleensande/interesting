#include "matrix.hpp"


// ------------------------- метод Гаусса ------------------------- //

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
    for (int i = 0; i < _rows - 1; i++) {
        make_diag_one_Gauss(i, fr);
        #pragma omp parallel for
        for (int k = i + 1; k < _rows; k++) {
            T coeff = _arr[k][i];
            substract_rows_Gauss(i, k, coeff, fr);
        }
    }
    make_diag_one_Gauss(_rows - 1, fr);
}

// выполняет обратный ход метода Гаусса
template <typename T>
void Matrix<T>::backward_Gauss(Matrix<double>* fr) {
    for (int i = _rows - 1; i > 0 ; i--) {
        #pragma omp parallel for
        for (int k = i - 1; k >= 0; k--) {
            T coeff = _arr[k][i];
            substract_rows_Gauss(i, k, coeff, fr);
        }
    }
}

// выполняет приведение матрицы к диагональному виду методом Гаусса
template <typename T>
void Matrix<T>::Gauss(Matrix<double>* fr) {
    forward_Gauss(fr);
    backward_Gauss(fr);
}

// меняет местами строки
template <typename T>
void Matrix<T>::swap_rows(int i1, int i2) {
    if (!check_i(i1) || !check_i(i2)) {
        return;
    }
    T sw;
    for (int j = 0; j < _cols; j++) {
        sw = _arr[i1][j];
        _arr[i1][j] = _arr[i2][j];
        _arr[i2][j] = sw;
    }
    return;
}

// ------------------------- метод Гаусса ------------------------- //






// --------------------- метод Гаусса-Жордана --------------------- //
template <typename T>
void Matrix<T>::Gauss_Jordan(Matrix<double>* fr) {
    
}

// --------------------- метод Гаусса-Жордана --------------------- //










// конструктор матрицы по строкам, столбцам и начальному значению ячеек
template <typename T>
Matrix<T>::Matrix(const int rows, const int cols, const T value) {
    _rows = rows;
    _cols = cols;
    _arr = new T*[rows];
    for (int i = 0; i < rows; i++) {
        _arr[i] = new T[cols];
    }
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] = value;
        }
    }
}

// копирующий конструктор
template <typename T>
Matrix<T>::Matrix(const Matrix& m) {
    // выделение памяти
    _rows = m.get_rows();
    _cols = m.get_cols();
    _arr = new T*[_rows];
    for (int i = 0; i < _rows; i++) {
        _arr[i] = new T[_cols];
    }

    // заполнение ее теми же значениями
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] = m.get(i, j);
        }
    }
}

// создает матрицу-вектор по переданному словарю, ключи отбрасываются
template <typename T>
Matrix<T>::Matrix(map<string, double> m) {
    _rows = m.size();
    _cols = 1;
    _arr = new T*[_rows];
    for (int i = 0; i < _rows; i++) {
        _arr[i] = new T[_cols];
    }
    
    int i = 0;
    for (auto it = m.begin(); it != m.end(); ++it) {
        _arr[i][0] = static_cast<T>(it->second);
        i++;
    }
}

// преобразует матрицу-вектор к словарю по переданным ключам
template <typename T>
map<string, T> Matrix<T>::to_map(vector<string> keys) const {
    map<string, T> result;

    if (_cols != 1) {
        cout << "ОШИБКА: только вектор может быть преобразован к словарю, cols = " << _cols << endl;
        return result;
    }

    if (static_cast<int>(keys.size()) != _rows) {
        cout << "ОШИБКА: размер вектора ключей = "<< keys.size() << " и количество строк в матрице = " << _rows  << " не совпадают"<< endl;
        return result;
    }

    for (int i = 0; i < _rows; i++) {
        result[keys[i]] = get(i, 0);
    }

    return result;
}

// конструктор присваивания
template <typename T>
Matrix<T>& Matrix<T>::operator= (const Matrix& m) {
    // проверка на самоприсваивание
    if (this == &m) {
        return *this;
    }

    // удаление старой памяти
    for (int i = 0; i < _rows; i++) {
        delete[] _arr[i];
    }
    delete[] _arr;

 
    // выделение новой памяти
    _rows = m.get_rows();
    _cols = m.get_cols();
    _arr = new T*[_rows];
    for (int i = 0; i < _rows; i++) {
        _arr[i] = new T[_cols];
    }

    // заполнение ее теми же значениями
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] = m.get(i, j);
        }
    }

    return *this;
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

// Возвращает матрицу, находящуюся в блоке размером rows*cols,
// левый верхний угол которого - i_lu и j_lu
template <typename T>
Matrix<T> Matrix<T>::get_block(int rows, int cols, int i_lu, int j_lu) const {
    Matrix<T> block(rows, cols, 0);
    
    if (!check_i(i_lu) || !check_j(j_lu)) {
        cout << "ОШИБКА: неверный номер строки = " << i_lu << " или столбца = " << j_lu << endl;
        return block;
    }

    if (!check_i(i_lu + rows - 1) || !check_j(j_lu + cols - 1)) {
        cout << "ОШИБКА: неверный размер блока = " << rows << " * " << cols << endl;
        return block;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            block.set(i, j, get(i + i_lu, j + j_lu));
        }
    }

    return block;
}

// Записывает block в блок исходной матрицы,
// левый верхний угол которого - i_lu и j_lu
template <typename T>
void Matrix<T>::set_block(Matrix<T> block, int i_lu, int j_lu) {
    if (!check_i(i_lu) || !check_j(j_lu)) {
        cout << "ОШИБКА: неверный номер строки = " << i_lu << " или столбца = " << j_lu << endl;
        return;
    }

    if (!check_i(i_lu + block.get_rows() - 1) || !check_j(j_lu + block.get_cols() - 1)) {
        cout << "ОШИБКА: неверный размер блока = " << block.get_rows() << " * " << block.get_cols() << endl;
        return;
    }

    for (int i = 0; i < block.get_rows(); i++) {
        for (int j = 0; j < block.get_cols(); j++) {
            set(i + i_lu, j + j_lu, block.get(i, j));
        }
    }
}


// Устанавливает элемент с индексами i и j в значение val
template <typename T>
void Matrix<T>::set(int i, int j, T val)  {
    if (!(check_i(i) && check_j(j))) {
        return;
    }

    _arr[i][j] = val;
}

// Проверяет на присутствие в матрице строку элемента
template <typename T>
bool Matrix<T>::check_i(int i) const { 
    if (i >= _rows) {
        cout << "ОШИБКА: слишком большой номер строки: " << i << endl;
        return false;
    } else if (i < 0) {
        cout << "ОШИБКА: слишком маленький номер строки: " << i << endl;
        return false;
    }

    return true;
}

// Проверяет на присутствие в матрице столбец элемента
template <typename T>
bool Matrix<T>::check_j(int j) const { 
    if (j >= _cols) {
        cout << "ОШИБКА: слишком большой номер столбца: " << j << endl;
        return false;
    } else if (j < 0) {
        cout << "ОШИБКА: слишком маленький номер столбца: " << j << endl;
        return false;
    }

    return true;
}

// Печатает переданное сообщение и саму матрицу
template <typename T>
void Matrix<T>::print(const string& message) const {
    cout.precision(3);
    cout << message << endl;
    for (int i = 0; i < _rows; i++) {
        cout << "| ";
        for (int j = 0; j < _cols; j++) {
            cout << setw(5) << _arr[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << endl;
}

// Удаляет матрицу и очищает память
template <typename T>
Matrix<T>::~Matrix() {
    if(_arr)
    {
        for (int i = 0; i < _rows; i++) {
            delete[] _arr[i];
        }
    }
    delete[] _arr;
    _arr = nullptr;
}

// печатает переданное сообщение, саму матрицу 
// и матрицу свободных членов
template <typename T>
void Matrix<T>::print_SLAU(Matrix<double>* fr, string message) const {
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

// находит максимальный по ширине элемент в столбце матрицы
template <typename T>
int Matrix<T>::max_width_in_col(int j) const {
    int max_width = 0;

    int current_width = 0;
    for (int i = 0; i < _rows; i++) {
        std::ostringstream strs;
        strs << get(i, j);
        std::string str = strs.str();
        current_width = str.length();

        if (current_width > max_width) {
            max_width = current_width;
        }
    }

    return max_width;
}

// Печатает матрицу в виде таблицы
template <typename T>
void Matrix<T>::nice_print(string message) const {
    if (message != "") {
        cout << message << " :" << endl;
    }

    // Расчет ширины каждого из столбцов по ширине максимально широкого элемента
    vector<int> fields_width(_cols);
    for (int i = 0; i < (static_cast<int> (_cols)); i++) {
        fields_width[i] = max_width_in_col(i) + 2;
    };

    // Расчет обшей ширины матрицы с учетом разделителей
    int matrix_width = 0;
    for (int w : fields_width) {
        matrix_width += w;
    }
    matrix_width += _cols + 1;    // на разделители

    // Печать таблицы
    for (int i = 0; i <= _rows; i++) {
        // Левый край линии
        if (i == 0) {
            cout << "┌";
        } else if (i == _rows) {
            cout << "└";
        } else {
            cout << "├";
        }


        // Сама линия
        for (int k = 0; k < _cols; k++) {
            for (int j = 0; j < fields_width[k]; j++) {
                cout << "─";
            }
            
            if (k != _cols - 1) {
                if (i == 0) {
                    cout << "┬";
                } else if (i == _rows) {
                    cout << "┴";
                } else {
                    cout << "┼";
                }
            }
        }


        // Правый край линии
        if (i == 0) {
            cout << "┐";
        } else if (i == _rows) {
            cout << "┘";
        } else {
            cout << "┤";
        }
        cout << endl;


        // Печать элементов
        if (i != _rows) {
            cout << "│";
            for (int j = 0; j < _cols; j++) {
                if (_arr[i][j] != 0) {
                    cout << " " << setw(fields_width[j] - 2) << _arr[i][j] << " │";
                } else {
                    cout << " " << setw(fields_width[j] - 2) << " " << " │";
                }
            }
            cout << endl;
        }
    }
}

// Умножение матрицы на число
template <typename T>
void Matrix<T>::operator*=(double n) {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] *= n;
        }
    }
}

// оператор += для матриц
template <typename T>
void Matrix<T>::operator+=(Matrix<T> m) {
    if (m.get_rows() != _rows || m.get_cols() != _cols) {
        cout << "ОШИБКА: размеры матриц для сложения не совпадают - ";
        cout << _rows  << " * " << _cols << " != " << m.get_rows() << " * " << m.get_cols() << endl;
    }

    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] += m.get(i, j);
        }
    }
}

// оператор -= для матриц
template <typename T>
void Matrix<T>::operator-=(Matrix<T> m) {
    if (m.get_rows() != _rows || m.get_cols() != _cols) {
        cout << "ОШИБКА: размеры матриц для вычитания не совпадают - ";
        cout << _rows  << " * " << _cols << " != " << m.get_rows() << " * " << m.get_cols() << endl;
    }

    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _arr[i][j] -= m.get(i, j);
        }
    }
}

// Умножение матрицы на число и возврат новой матрицы
template <typename T>
Matrix<T> Matrix<T>::operator*(double n) {
    Matrix<T> result = (*this);

    result *= n;

    return result;
}

// Вычитание матриц
template <typename T>
Matrix<T> Matrix<T>::operator-(Matrix<T> m) {
    Matrix<T> result = (*this);

    result += m * (-1);

    return result;
}

// Сложение матриц
template <typename T>
Matrix<T> Matrix<T>::operator+(Matrix<T> m) {
    Matrix<T> result = (*this);

    result += m;

    return result;
}

// Деление матрицы на число
template <typename T>
Matrix<T> Matrix<T>::operator/(double n) {
     Matrix<T> result = (*this);

    result *= (1 / n);

    return result;
}

// Расчет равномерной нормы вектора
template <typename T> 
double Matrix<T>::inf_norm() {
    if (get_cols() != 1) {
        cout << "ОШИБКА: вектор имеет более одного столбца, а именно: " << get_cols() << endl;
        return -1;
    }

    double max_el = 0;
    for (int i = 0; i < get_rows(); i++) {
        if(get(i, 0) > max_el) {
            max_el = get(i, 0);
        }
    }

    return max_el;
}


template class Matrix<double>;
template class Matrix<int>;

