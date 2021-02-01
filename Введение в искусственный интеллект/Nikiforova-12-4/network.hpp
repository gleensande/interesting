#include <iostream>
#include <fstream>
#include <vector>
#include "neuron.hpp"

using namespace std;

class Network {
 private:
    vector<Neuron> neurons;
    ofstream weights_file;
    double learn_coef;
    double neighb_level;
 public:
    Network(int neurons_num);
    void set_rand_weights();
    double G_func(int neuron_i);
    void change_weights();
    void print_current_weights();
    void learn(vector< pair<double, double> >& X_learn);
    ~Network();
};