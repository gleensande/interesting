#include <iostream>
#include <cmath>
#include "matrix.hpp"
#include "csv_parser.hpp"

#define ABS(a) (((a) < 0) ? -(a) : (a))

double analytics(double x) {
    double l_1 = (5 + sqrt(17)) / 4;
    double l_2 = (5 - sqrt(17)) / 4;
    double c_1 = -13.9;
    double c_2 = 117;
    return c_1 * exp(l_1 * x) + c_2 * exp(l_2 * x);
}

Matrix<double> lin_experiment(map<string, double>& C, double N) {
    // получение матрицы и коэффициента
    // для анасамблирования
    double l = 1.0 / N;

    double a[2][2];
    a[0][0] = -2/l + 2.5 + l/3;
    a[0][1] = 2/l - 2.5 + l/6;
    a[1][0] = 2/l + 2.5 + l/6;
    a[1][1] = -2/l - 2.5 + l/3;

    double coef = 2;


    // анасамблирование всех матриц
    // создание
    Matrix<double> A(N + 3, N + 3);
    Matrix<double> R(N + 3, 1);

    // наложение граничных условий
    R.set(N + 1, 0, C["dU_0"]);
    R.set(N + 2, 0, C["U_1"]);

    A.set(N + 1, N + 1, 1);
    A.set(N + 2, N, 1);

    // учет границ
    A.set(0, N + 1, -coef);
    A.set(N, N + 2, coef);

    // добавление КЭ-матрицы по диагонали
    for (int i = 0; i <= N - 1; i++) {
        A.set(i, i, A.get(i, i) + a[0][0]);
        A.set(i, i + 1, A.get(i, i + 1) + a[0][1]);
        A.set(i + 1, i, A.get(i + 1, i) + a[1][0]);
        A.set(i + 1, i + 1, A.get(i + 1, i + 1) + a[1][1]);
    }

    // решение методом Гаусса
    A.Gauss(&R);

    return R;
}

Matrix<double> quadr_experiment(map<string, double>& C, double N) {
    // получение матрицы и коэффициента
    // для анасамблирования
    double l = 1.0 / N;

    double a[3][3];

    // заполняем третью строку сначала, чтобы ее можно было вычитать
    a[2][0] = 16.0/(3.0 * l) + 10.0/3.0 + l/15.0;
    a[2][1] = 16.0/(3.0 * l) - 10.0/3.0 + l/15.0;
    a[2][2] = -32.0/(3.0 * l) + (8.0 * l)/15.0;


    // заполнение первой и второй строки начальными значениями
    a[0][0] = -14.0/(3.0 * l) + 2.5 + (2.0 * l)/15.0;
    a[0][1] = -2.0/(3.0 * l) + 5.0/6.0 - l/30.0;
    a[0][2] = 16.0/(3.0 * l) - 10.0/3.0 + l/15.0;
    
    a[1][0] = -2.0/(3.0 * l) - 5.0/6.0 - l/30.0;
    a[1][1] = -14.0/(3.0 * l) - 2.5 + (2.0 * l)/15.0;
    a[1][2] = 16.0/(3.0 * l) + 10.0/3.0 + l/15.0;

    // расчет коэффициентов и прямой ход метода Гаусса
    //double k_1 = (-80 + 50 * l + l * l)/(-160 + 8 * l * l);

    double k1 = -a[0][2]/a[2][2];
    
    for (int i = 0; i < 3; i++) {
        a[0][i] += a[2][i] * k1;
    }

    //double k_2 = (-80 - 50 * l + l * l)/(-160 + 8 * l * l);

    double k2 = -a[1][2]/a[2][2];
    
    for (int i = 0; i < 3; i++) {
        a[1][i] += a[2][i] * k2;
    }


    double coef = 2;


    // анасамблирование всех матриц
    // создание
    Matrix<double> A(N + 3, N + 3);
    Matrix<double> R(N + 3, 1);

    // наложение граничных условий
    R.set(N + 1, 0, C["dU_0"]);
    R.set(N + 2, 0, C["U_1"]);

    A.set(N + 1, N + 1, 1);
    A.set(N + 2, N, 1);

    // учет границ
    A.set(0, N + 1, -coef);
    A.set(N, N + 2, coef);

    // добавление КЭ-матрицы по диагонали
    for (int i = 0; i <= N - 1; i++) {
        A.set(i, i, A.get(i, i) + a[0][0]);
        A.set(i, i + 1, A.get(i, i + 1) + a[0][1]);
        A.set(i + 1, i, A.get(i + 1, i) + a[1][0]);
        A.set(i + 1, i + 1, A.get(i + 1, i + 1) + a[1][1]);
    }

    // решение методом Гаусса
    A.Gauss(&R);

    return R;
}

Matrix<double> additional_experiment(map<string, double>& C, double N) {
    // получение матрицы и коэффициента
    // для анасамблирования
    double l = 1 / N;

    double a[2][2];
    a[0][0] = -2/l + 2.5 + l/3;
    a[0][1] = 2/l - 2.5 + l/6;
    a[1][0] = 2/l + 2.5 + l/6;
    a[1][1] = -2/l - 2.5 + l/3;

    double coef = 2;


    // анасамблирование всех матриц
    // создание
    Matrix<double> A(N + 3, N + 3);
    Matrix<double> R(N + 3, 1);

    // наложение граничных условий
    R.set(N + 1, 0, C["dU_0"]);
    R.set(N + 2, 0, 0);

    A.set(N + 1, N + 1, 1);
    A.set(N + 2, N, -1);
    A.set(N + 2, N + 2, 1);

    // учет границ
    A.set(0, N + 1, -coef);
    A.set(N, N + 2, coef);

    // добавление КЭ-матрицы по диагонали
    for (int i = 0; i <= N - 1; i++) {
        A.set(i, i, A.get(i, i) + a[0][0]);
        A.set(i, i + 1, A.get(i, i + 1) + a[0][1]);
        A.set(i + 1, i, A.get(i + 1, i) + a[1][0]);
        A.set(i + 1, i + 1, A.get(i + 1, i + 1) + a[1][1]);
    }

    // решение методом Гаусса
    A.Gauss(&R);

    return R;
}

void accuracy(Matrix<double>& R, string filename) {
    std::ofstream result_file;
    result_file.open(filename);
    if (!result_file) {
        cout << "ОШИБКА: Невозможно открыть файл для записи погрешностей: " << filename << endl;
        return;
    }

    double N = R.get_rows() - 3;
    double l = 1 / N;
    double max_acc = 0;
    for (int i = 0; i < N + 1; i++) {
        double time = i * l;
        result_file << time << "\t" << ABS(R.get(i, 0) - analytics(time)) << endl;
        if (ABS(R.get(i, 0) - analytics(time)) > max_acc) {
            max_acc = ABS(R.get(i, 0) - analytics(time));
        }
    }

    result_file.close();
    cout << filename << ":" << endl;
    cout << max_acc << endl << endl;
}

void graph(Matrix<double>& R, string filename) {
    std::ofstream result_file;
    result_file.open(filename);
    if (!result_file) {
        cout << "ОШИБКА: Невозможно открыть файл для записи результатов: " << filename << endl;
        return;
    }

    double N = R.get_rows() - 2;
    double l = 1 / N;
    for (int i = 0; i < N; i++) {
        result_file << i * l << "\t" << R.get(i, 0) << endl;
    }

    result_file.close();
}

int main() {
    map<string, double> C = create_map_from_file("constants.dat");

    Matrix<double> R1 = lin_experiment(C, C["N_1"]);
    graph(R1, "res/lin_result_N1.dat");
    accuracy(R1, "res/lin_accuracy_N1.dat");

    Matrix<double> R2 = lin_experiment(C, C["N_2"]);
    graph(R2, "res/lin_result_N2.dat");
    accuracy(R2, "res/lin_accuracy_N2.dat");

    /*Matrix<double> R3 = additional_experiment(C, C["N_2"]);
    graph(R3, "res/additional_N2.dat");*/

    Matrix<double> R4 = quadr_experiment(C, C["N_1"]);
    graph(R4, "res/quadr_result_N1.dat");
    accuracy(R4, "res/quadr_accuracy_N1.dat");

    Matrix<double> R5 = quadr_experiment(C, C["N_2"]);
    graph(R5, "res/quadr_result_N2.dat");
    accuracy(R5, "res/quadr_accuracy_N2.dat");

    return 0;
}