#ifndef FUNCTIONS_HPP_
#define FUNCTIONS_HPP_

#include <Eigen/Dense>
#include <cmath>

using Vector = Eigen::VectorXd;

//Define the 1-dim ODE test function
Vector df1(const double& t, const Vector& y);
//Define the solution to assess convergence of the method
Vector f1(const double& t);

//Define the 2-dim ODE test function
Vector df2(const double& t, const Vector& y);
//Define the solution to assess convergence of the method
Vector f2(const double& t);


#endif