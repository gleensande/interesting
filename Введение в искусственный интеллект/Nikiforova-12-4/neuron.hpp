#include <iostream>

using namespace std;

class Neuron {
 private:
    pair<double, double> weights;
    pair<double, double> current_x;
    int real_i;
    double potential;
 public:
    void set_weights(pair<double, double> new_weights);
    void set_current_x(pair<double, double> new_current_x);
    double distance();
    void set_real_i(int new_real_i);
    int get_real_i();
    pair<double, double> diff();
    pair<double, double> get_weights();
    void set_potential(double new_potential);
    double get_potential();
};