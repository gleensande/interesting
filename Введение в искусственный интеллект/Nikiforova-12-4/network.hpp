#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "neuron.hpp"

class Network {
 private:
    std::vector<Neuron> Neurons;
    std::ofstream output_file;
    double n;
    double sigma;
    int Swk;
 public:
    Network(int neurons_num);
    ~Network();
    int define_winner(std::vector<double>& _X);
    void learn(std::vector<std::vector <double> >& X_learn);
    void print_neurons_weights(std::ofstream& _file);
    void check_koeffs();
};