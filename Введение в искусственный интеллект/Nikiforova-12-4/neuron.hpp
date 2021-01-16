#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>

class Neuron {
 private:
    std::vector<double> W;
    int input_num;
    int distance_to_winner;
 public:
    Neuron(int _input_num);
    double output_signal(std::vector<double>& _X);
    void change_weights(std::vector<double>& _X, double _n, double _sigma);
    int get_input_num();
    std::vector<double> get_weights();
    void print_weights();
    void set_rand_W();
    double calculate_distance(std::vector<double>& _X);
    std::vector<double> vector_normalize(std::vector<double>& _vector);
    void set_distance_to_winner(int _distance);
    int get_distance();
    void print_weights(std::ofstream& file);
};
