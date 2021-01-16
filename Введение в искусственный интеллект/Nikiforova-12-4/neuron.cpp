#include "neuron.hpp"

Neuron::Neuron(int _input_num) {
    input_num = _input_num;
    W.resize(input_num);
}

double Neuron::output_signal(std::vector<double>& X) {
    double sum = 0.0;
    for(int i = 0; i < input_num; i++) {
        sum += W[i] * X[i];
    }
    return sum;
}

void Neuron::change_weights(std::vector<double>& X, double n, double sigma) {
    for(int i = 0; i < input_num; i++) {
        W[i] = W[i] + n * exp(-(distance_to_winner * distance_to_winner) / (2 * sigma * sigma)) * (X[i] - W[i]);
    }
}

int Neuron::get_input_num() {
    return input_num;
}

std::vector<double> Neuron::get_weights() {
    return W;
}

void Neuron::print_weights() {
    for(int i = 0; i < W.size(); i++) {
        std::cout << "W" << i+1 << " = " << W[i] << std::endl;
    }
}

void Neuron::set_distance_to_winner(int _distance) {
    distance_to_winner = _distance;
}

int Neuron::get_distance() {
    return distance_to_winner;
}

void Neuron::set_rand_W() {
    double temp;
    for(int i = 0; i < input_num; i++) {
        W[i] = rand() % 6;
        temp = rand() % 100;
        temp /= 100;
        W[i] -= 3;
        W[i] += temp;
    }
    //W = vector_normalize(W);
}

// Нормализация вектора
std::vector<double> Neuron::vector_normalize(std::vector<double>& _vector) {
    double sum = 0.0;
    std::vector<double> vector = _vector;
    for(int i = 0; i < vector.size(); i++) {
        sum += (vector[i] * vector[i]);
    }
    double root = sqrt(sum);
    for(int i = 0; i < vector.size(); i++) {
        vector[i] /= root;
    }
    return vector;
}

//Эвклидова мера
double Neuron::calculate_distance(std::vector<double>& _X) {
    double sum = 0.0;
    std::vector<double> X = _X;
    for (int i = 0; i < input_num; i++) {
        sum += ((X[i] - W[i]) * (X[i] - W[i]));
    }
    sum = sqrt(sum);
    return sum;
}

void Neuron::print_weights(std::ofstream& weights_file) {
        weights_file << W[0] << " " << W[1] << std::endl;
}