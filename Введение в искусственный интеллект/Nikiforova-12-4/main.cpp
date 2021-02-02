#include <iostream>
#include <fstream>
#include <vector>
#include "network.hpp"

using namespace std;

vector< pair<double, double> > read_learn_data() {
    ifstream data;
    data.open("coordinates.dat");

    vector< pair<double, double> > X_learn;
    double x1, x2;
    while (data >> x1 >> x2) {
        X_learn.push_back(make_pair(x1, x2));
    }

    data.close();
    return X_learn;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: ./a.out neurons_num" << endl;
        return 0;
    }

    vector< pair<double, double> > X_learn = read_learn_data();

    Network N(atoi(argv[1]));
    for (int i = 0; i < 5; i++) {
        N.learn(X_learn, i, 5, ((i < 2) ? true : false));
    }
}