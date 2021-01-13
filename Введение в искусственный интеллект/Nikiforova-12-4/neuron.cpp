#include "neuron.hpp"
#include <ctime>
#include <fstream>

// конструктор нейрона по _inputs_num - количеству входов
void Neuron::set_input_num(int _inputs_num) {
    inputs_num = _inputs_num;
    W.resize(inputs_num + 1);
}

// функция активации, X - вектор входных сигналов размера inputs_num
double Neuron::activation_func(vector<double>& X) {
    if (X.size() != inputs_num) {
        cout << "ERROR: X.size() != inputs_num" << endl;
        return 0;
    }

    return u_summ(X);
}

// сумма сигналов * на веса u, X - вектор входных сигналов размера inputs_num 
double Neuron::u_summ(vector<double>& X) {
    if (X.size() != inputs_num) {
        cout << "ERROR: X.size() != inputs_num" << endl;
        return 0;
    }

    double u = 0;

    for (size_t i = 0; i < inputs_num; i++) {
        u += X[i] * W[i];
    }

    return u;
}
