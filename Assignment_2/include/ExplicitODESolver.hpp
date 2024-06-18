#ifndef EXPLICIT_ODE_SOLVER_HPP_
#define EXPLICIT_ODE_SOLVER_HPP_

#include <Eigen/Dense>
#include <vector>
#include <functional>
#include <fstream>
#include <chrono>
#include <cmath>
#include <numeric>

using uint = unsigned int;
using Vector = Eigen::VectorXd;
using Matrix = Eigen::MatrixXd;

class ExplicitODESolver {
protected:
    // Define the Butcher tableau
    //They are defined non const in order to define the proper Tableau for each solver
    Matrix A;   //Matrix for stages
    Vector b;   //Coefficients for updating the solution
    Vector c;   //Coefficients for updating time steps
    // Store the function you want to compute
    const std::function<Vector(double, const Vector&)> func;
    // Define the vectors timestep and solstep, useful for printing to CSV
    std::vector<double> timeStep;
    std::vector<Vector> solStep;

public:
    //Define the constructor
    ExplicitODESolver(const Matrix& A, const Vector& b, const Vector& c, const std::function<Vector(double, const Vector&)>& f):A(A), b(b), c(c), func(f) {}
    //Define the virtual destroyer
    virtual ~ExplicitODESolver()=default;
    // Define the solver method as void, as it only modifies the instances of the class
    void solve(const double &t0, const double& tf, const Vector& y0, const uint numsteps);
    //Getter methods to return protected attributes of the class
    std::vector<double> getTimeSteps() const {return timeStep;}

    std::vector<Vector> getSolSteps() const {return solStep;}

    // Write solutions to a CSV file
    void writeCSV(const std::string& filename) const;

    // Compute the maximum error using the infinity norm
    double computeInfinityNormError(const std::function<Vector(double)>& exactSol) const;

    // Function to calculate the mean execution time over 100 runs
    double meanExeTime(const double &t0, const double &tf, const Vector& y0, const uint &numSteps);

    //Function to calculate the single execution time of the solving algorithm
    double singleExeTime(const double &t0, const double &tf, const Vector& y0, const uint &numSteps);

    // Function to compute the empirical order of convergence
    double computeEmpiricalOrder(const double &t0, const double &tf, const Vector &y0, const uint &numSteps, const std::function<Vector(double)>& exactSol);
};

// Derived classes for specific solvers
class ForwardEuler : public ExplicitODESolver {
public:
    ForwardEuler(const std::function<Vector(double, const Vector&)>& f);
};

class RK4 : public ExplicitODESolver {
public:
    RK4(const std::function<Vector(double, const Vector&)>& f);
};

class MidPoint : public ExplicitODESolver {
public:
    MidPoint(const std::function<Vector(double, const Vector&)>& f);
};

#endif

