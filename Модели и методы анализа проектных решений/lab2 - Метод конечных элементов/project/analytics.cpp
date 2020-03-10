#include "matrix.hpp"
#include <cmath>
#include <iostream>

// c++ analytics.cpp matrix.cpp -o an.out

using namespace std;

void check_right(double C_1, double C_2, double l_1, double l_2) {
    double u_1 = C_1 * exp(l_1) + C_2 * exp(l_2);
    cout << "u[1] = " << u_1 << endl;
}

void check_left(double C_1, double C_2, double l_1, double l_2) {
    double u_0 = C_1 * l_1 + C_2 * l_2;
    cout << "u[0] = " << u_0 << endl;
}

void check_additional() {
    double pt = 0.95122, pu = -29.6227, t = 0.97561, u = -30.3633;

    double du = (u - pu) / (t - pt);
    cout << "du[1] = " << du << endl;
    cout << "u[1] = " << u << endl;
}

int main() {
    double l_1 = (5 + sqrt(17)) / 4;
    double l_2 = (5 - sqrt(17)) / 4;

    Matrix<double> A(2, 2);
    A.set(0, 0, exp(l_1));
    A.set(0, 1, exp(l_2));
    A.set(1, 0, l_1);
    A.set(1, 1, l_2);

    Matrix<double> C(2, 1);
    C.set(0, 0, 10);
    C.set(1, 0, -6);

    A.print(&C);
    A.Gauss(&C);
    A.print(&C);

    check_right(C.get(0, 0), C.get(1, 0), l_1, l_2);
    check_left(C.get(0, 0), C.get(1, 0), l_1, l_2);
    check_additional();

    return 0;
}

