#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Perceptron {
 private:
    int inputs_num;
    vector<double> W;
 public:
    Perceptron(int _inputs_num);
    bool activation_func(vector<double>& X);
    double u_summ(vector<double>& X);
    void learn(vector< vector<double> >& X_learn, vector<bool>& D, int N_max);
    double aim_func(vector<bool>& D, vector<bool>& Y);
};
