#include "network.hpp"
#include <random>
#include <ctime>
#include <algorithm>
#include <functional>

#define LEARN_COEF_MAX 5.0
#define LEARN_COEF_MIN 0.1
#define NEIGHB_LEVEL_MAX 5.0
#define NEIGHB_LEVEL_MIN 0.01
#define POTENTIAL_MIN 0.75
#define POTENTIAL_MAX 1

using namespace std;

Network::Network(int neurons_num) {
    // srand(time(NULL));
    neurons.resize(neurons_num);
    for (int i = 0; i < neurons_num; i++) {
        neurons[i].set_real_i(i);
        neurons[i].set_potential(POTENTIAL_MIN);
    }
    weights_file.open("weights.dat");
    learn_coef = LEARN_COEF_MAX;
    neighb_level = NEIGHB_LEVEL_MAX;
    set_rand_weights();
}

void Network::set_rand_weights() {
    double x_1, x_2;
    for (int i = 0; i < neurons.size(); i++) {
        x_1 = rand() % 4 - 2;    // по X_1 = [-2, 2]
        x_2 = rand() % 5 - 2;    // по X_2 = [-2, 3]

        neurons[i].set_weights(make_pair(x_1, x_2));
    }
}

void Network::print_current_weights() {
    pair<double, double> weights;
    for (int i = 0; i < neurons.size(); i++) {
        weights = neurons[i].get_weights();
        weights_file << neurons[i].get_real_i() << " " << weights.first << " " << weights.second << endl;
    }
    weights_file << endl;
}

double Network::G_func(int neuron_i) {
    return exp(-neuron_i / neighb_level);
}

void Network::change_weights(bool potentials) {
    pair<double, double> w;

    for (int i = 0; i < neurons.size(); i++) {
        w = neurons[i].get_weights();

        // ПОТЕНЦИАЛЫ
        if (potentials) {
            if (i == 0) {
                neurons[i].set_potential(neurons[i].get_potential() - POTENTIAL_MIN);
            } else {
                if (neurons[i].get_potential() < 1) {
                    neurons[i].set_potential(neurons[i].get_potential() + 1.0 / ((double)neurons.size()) );
                }
            }
        }
        

        w.first += learn_coef * G_func(i) * neurons[i].diff().first;
        w.second += learn_coef * G_func(i) * neurons[i].diff().second;

        neurons[i].set_weights(w);
    }
}

void Network::learn(vector< pair<double, double> >& X_learn, int cur_iter, int iter_max, bool potentials) {
    print_current_weights();

    if (!potentials) {
        for (int i = 0; i < neurons.size(); i++) {
            neurons[i].set_potential(POTENTIAL_MIN);
        }
    }

    int winner_index;
    for (int i = 0; i < X_learn.size(); i++) {
        // устанавливаем всем нейронам текущее значение сигналов
        for (int j = 0; j < neurons.size(); j++) {
            neurons[j].set_current_x(X_learn[i]);
        }

        // сортируем все нейроны по расстоянию до сигнала с помощью лямбда-функции
        std::sort(neurons.begin(), neurons.end(), [](Neuron a, Neuron b) {
            return a.distance() < b.distance();
        });

        // меняем веса победителя и соседей
        change_weights(potentials);

        // выводим веса в файл, чтобы потом визуализировать обучение
        print_current_weights();

        // уменьшаем коэффициенты learn_coef и neighbourhood_level
        learn_coef = LEARN_COEF_MAX * pow(LEARN_COEF_MIN / LEARN_COEF_MAX, ((double)((cur_iter * X_learn.size()) + i + 1)) / ((double)X_learn.size() * iter_max));
        neighb_level = NEIGHB_LEVEL_MAX * pow(NEIGHB_LEVEL_MIN / NEIGHB_LEVEL_MAX, ((double)((cur_iter * X_learn.size()) + i + 1)) / ((double)X_learn.size() * iter_max));
    }
}

Network::~Network() {
    weights_file.close();
}