#include "neuron.hpp"
#include <cmath>

#define POTENTIAL_MIN 0.75

void Neuron::set_weights(pair<double, double> new_weights) {
    weights = new_weights;
}

pair<double, double> Neuron::get_weights() {
    return weights;
}

void Neuron::set_real_i(int new_real_i) {
    real_i = new_real_i;
}

int Neuron::get_real_i() {
    return real_i;
}

void Neuron::set_current_x(pair<double, double> new_current_x) {
    current_x = new_current_x;
}

double Neuron::distance() {
    // ПОТЕНЦИАЛЫ
    /*if (potential < POTENTIAL_MIN) {
        return 100;
    }*/

    return sqrt(
        (weights.first - current_x.first) * (weights.first - current_x.first) +
        (weights.second - current_x.second) * (weights.second - current_x.second)
    );
}

pair<double, double> Neuron::diff() {
    return make_pair(current_x.first - weights.first, current_x.second - weights.second);
}

void Neuron::set_potential(double new_potential) {
    potential = new_potential;
}

double Neuron::get_potential() {
    return potential;
}
