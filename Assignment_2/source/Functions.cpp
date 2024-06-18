#include "Functions.hpp"

Vector df1(const double& t, const Vector& y) {
    //Input arguments are set to void because it's possible not to use both of them
    (void)y;
    (void)t;
    // Example: Define a simple system of ODEs dy/dt = -y
    Vector dydt(1);
    dydt[0] = -y[0];
    return dydt;
}

Vector f1(const double& t){
    Vector y(1);
    y[0] = std::exp(-t);
    return y;
}

Vector df2(const double& t, const Vector& y) {
    //Input arguments are set to void because it's possible not to use both of them
    (void)y;
    (void)t;
    // Example: Define a simple system of ODEs
    Vector dydt(2);
    dydt[0] = y[1];   // dy0/dt = y1
    dydt[1] = -y[0];  // dy1/dt = -y0
    return dydt;
}

Vector f2(const double& t){
    Vector y(2);
    y[0] = std::cos(t);
    y[1] = -std::sin(t);
    return y;
}
