#include "neuron.hpp"
#include <ctime>

// конструктор нейрона по _inputs_num - количеству входов
Perceptron::Perceptron(int _inputs_num) {
    inputs_num = _inputs_num;
    W.resize(inputs_num + 1);

    weights_file.open("weights.dat");
    if (!weights_file) {
        cout << "ERROR: can't open file 'weights.dat'" << endl;
        return;
    }

    for (size_t i = 0; i < inputs_num + 1; i++) {
        W[i] = rand() % 1000;
        W[i] /= 1000;
    }

    cout << "Start W: ";
    cout << W[inputs_num] << " ";
    weights_file << W[inputs_num] << " ";
    for (size_t i = 0; i < inputs_num; i++) {
        weights_file << W[i] << " ";
        cout << W[i] << " ";
    }
    weights_file << endl;
    cout << endl << endl;
}

// функция активации, X - вектор входных сигналов размера inputs_num
bool Perceptron::activation_func(vector<double>& X) {
    if (X.size() != inputs_num) {
        cout << "ERROR: X.size() != inputs_num" << endl;
        return 0;
    }

    if (u_summ(X) >= 0) {
        return true;
    } else {
        return false;
    }
}

// сумма сигналов * на веса u, X - вектор входных сигналов размера inputs_num 
double Perceptron::u_summ(vector<double>& X) {
    if (X.size() != inputs_num) {
        cout << "ERROR: X.size() != inputs_num" << endl;
        return 0;
    }

    double u = 0;

    for (size_t i = 0; i < inputs_num; i++) {
        u += X[i] * W[i];
    }
    u += W[inputs_num]; // поляризатор

    return u;
}

// обучение, X_learn - вектор входных сигналов размера обучающей выборки (каждый подвектор размера inputs_num), 
// D - вектор эталонных выходных сигналов размера обучающей выборки, N_max - предельное число циклов обучения
void Perceptron::learn(vector< vector<double> >& X_learn, vector<bool>& D, int N_max, double coef) {
    if (D.size() != X_learn.size()) {
        cout << "ERROR: D.size() != X_learn.size()" << endl;
        return;
    }

    // для подсчета функции активации
    vector<bool> Y(D.size());

    for (size_t k = 0; k < X_learn.size(); k++) {
        bool stop = false;

        for (size_t i = 0; i < N_max && !stop; i++) {
            bool y = activation_func(X_learn[k]);

            if (y != D[k]) {
                if (y == false) {
                    for (size_t j = 0; j < inputs_num; j++) {
                        W[j] += coef * X_learn[k][j];
                    }
                    W[inputs_num] += coef * 1;
                } else {
                    for (size_t j = 0; j < inputs_num; j++) {
                        W[j] -= coef * X_learn[k][j];
                    }
                    W[inputs_num] -= coef * 1;
                }

                cout << "N = " << i << ". ";
                cout << W[inputs_num] << " ";
                weights_file << W[inputs_num] << " ";
                for (size_t i = 0; i < inputs_num; i++) {
                    weights_file << W[i] << " ";
                    cout << W[i] << " ";
                }
                weights_file << endl;
                cout << endl;

                // Подсчет функции активации для наблюдения за ее изменением
                for (size_t i = 0; i < D.size(); i++) {
                    Y[i] = activation_func(X_learn[i]);
                }
                cout << "Aim func = " << aim_func(D, Y);
                cout << endl << endl;
            } else {
                stop = true;
            }
        }   
    }

    return;
}

// целевая функция, D - вектор эталонных выходных сигналов размера обучающей выборки,
// Y - вектор реальных выходных сигналов размера обучающей выборки
double Perceptron::aim_func(vector<bool>& D, vector<bool>& Y) {
    if (D.size() != Y.size()) {
        cout << "ERROR: D.size() != Y.size()" << endl;
        return 0;
    }
    
    double E = 0;

    for (size_t k = 0; k < D.size(); k++) {
        E += (Y[k] - D[k]) * (Y[k] - D[k]);
    }

    return 0.5 * E;
}

bool Perceptron::test(vector< vector<double> >& X_test, vector<bool>& D_test) {
    bool r = true;

    cout << "Testing:" << endl;
    // тестирование нейрона
    for (size_t i = 0; i < D_test.size(); i++) {
        bool result = activation_func(X_test[i]);
        if (result == D_test[i]) {
            cout << "1";
        } else {
            cout << "0";
            r = false;
        }
    }
    cout << endl;

    return r;
}

Perceptron::~Perceptron() {
    weights_file.close();
}