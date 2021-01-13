#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Neuron {
 private:
    int inputs_num;
    vector<double> W;
 public:
    void set_input_num(int _inputs_num);
    double activation_func(vector<double>& X);
    double u_summ(vector<double>& X);
};
