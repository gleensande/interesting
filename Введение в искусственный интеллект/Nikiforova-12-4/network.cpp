#include "network.hpp"
#define INPUT_NUMS 2
#define N 0.5
#define SIGMA 0.4
#define SWK 4

Network::Network(int neurons_num) {
    // Файл для записи выходных данных 
    output_file.open("test.dat");
    if (!output_file) {
        std::cout << "ERROR: can't open file 'test.dat'" << std::endl;
        return;
    }

    srand(time(NULL));
    for(int i = 0; i < neurons_num; i++) {
        Neuron Ner(INPUT_NUMS);
        Ner.set_rand_W();
        Neurons.push_back(Ner);
    }

    n = neurons_num;
}

Network::~Network() {
    output_file.close();
}

// Определение нейрона-победителя и переопределение его весов
int Network::define_winner(std::vector<double>& X) {
    double min_distance = 100000.0;
    double temp_distance = 0.0;
    int winner_index = 0;
    for(int i = 0; i < Neurons.size(); i++) {
        temp_distance = Neurons[i].calculate_distance(X);
        if(temp_distance < min_distance) {
            min_distance = temp_distance;
            winner_index = i;
        }
    }
    output_file << X[0] << " " << X[1] << " " << winner_index << " " << std::endl;
    return winner_index;
}

void Network::learn(std::vector<std::vector <double> >& X_learn) {
    std::vector<double> X;
    int winner_index;
    std::ofstream weights_file;
    
    weights_file.open("weights.dat");
    if (!weights_file) {
        std::cout << "ERROR: can't open file 'weights.dat'" << std::endl;
        return;
    }

    for(int i = 0; i < X_learn.size(); i++) {
        //...
    }
    weights_file.close();
}

void Network::print_neurons_weights(std::ofstream& weights_file) {
    for(int i = 0; i < Neurons.size(); i++) {
        Neurons[i].print_weights(weights_file);
    }
}