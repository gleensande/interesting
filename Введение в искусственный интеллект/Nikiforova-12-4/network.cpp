#include "network.hpp"

Network::Network(size_t _neuron_num){
    neurons.resize(_neuron_num);
    neuron_num = _neuron_num;
}

void Network::learn(vector< vector<double> >& X_learn, int N){
    cout << "Learning:" << endl;
}
