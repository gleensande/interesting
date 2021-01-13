#pragma once
#include <vector>
#include <iostream>
#include "neuron.hpp"

using namespace std;

class Network {
 private:
    size_t neuron_num;
    vector<Neuron> neurons;
 public:
    Network(size_t _neuron_num);
    void learn(vector< vector<double> >& X_learn, int N);
};
