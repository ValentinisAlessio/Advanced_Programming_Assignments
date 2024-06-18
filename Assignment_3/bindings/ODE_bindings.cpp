#include "ExplicitODESolver.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

/////////////////////////////////////////////////////////////
////////////////////// BINDINGS /////////////////////////////
/////////////////////////////////////////////////////////////

namespace py = pybind11;

PYBIND11_MODULE(ODE, m) {
    m.doc() = "C++ library for solving ODEs using explicit methods";

    py::class_<ExplicitODESolver>(m, "ExplicitODESolver")
        .def(py::init<const Matrix&, const Vector&, const Vector&, const std::function<Vector(double, const Vector&)>&>(),
            py::arg("A"), py::arg("b"), py::arg("c"), py::arg("df"),
            R"(Constructor for the ExplicitODESolver class

            Parameters:
                A (numpy.array): Matrix for stages
                b (numpy.array): Coefficients for updating the solution
                c (numpy.array): Coefficients for updating time steps
                df (function): Function to compute the derivative of the solution
            )")
        .def("solve", &ExplicitODESolver::solve,
            py::arg("t0"), py::arg("tf"), py::arg("y0"), py::arg("num_steps"), 
            R"(Solve the ODE using the Butcher tableau and store time and solution vectors
            
            Parameters:
                t0 (float): Initial time
                tf (float): Final time
                y0 (list or numpy.array): Initial condition
                num_steps (int): Number of steps to  (it has to be a positive integer)
            
            Returns:
                void
            )")
        .def("writeCSV", &ExplicitODESolver::writeCSV,
            py::arg("filename"),
            R"(Write the time and solution vectors to a CSV file
            
            Parameters:
                filename (string): Name of the CSV file
            )")
        .def("computeInfinityNormError", &ExplicitODESolver::computeInfinityNormError,
            py::arg("exactSol"),
            R"(Compute the infinity norm error and store the error vector
            
            Parameters:
                exactSol (function): Exact solution of the differential problem)")
        .def("meanExeTime", &ExplicitODESolver::meanExeTime,
            py::arg("t0"), py::arg("tf"), py::arg("y0"), py::arg("num_steps"),
            R"(Compute the mean execution time over 100 runs
            Parameters:
                t0 (float): Initial time
                tf (float): Final time
                y0 (list or numpy.array): Initial condition
                num_steps (int): Number of steps to  (it has to be a positive integer
                
            Returns:
                double: Mean execution time over 100 runs in microseconds
            )")
        .def("singleExeTime", &ExplicitODESolver::singleExeTime,
            py::arg("t0"), py::arg("tf"), py::arg("y0"), py::arg("num_steps"),
            R"(Compute the single execution time of the solving algorithm
            Parameters:
                t0 (float): Initial time
                tf (float): Final time
                y0 (list or numpy.array): Initial condition
                num_steps (int): Number of steps to  (it has to be a positive integer
                
            Returns:
                double: Single execution time in microseconds
            )")    
        .def("computeEmpiricalOrder", &ExplicitODESolver::computeEmpiricalOrder,
            py::arg("t0"), py::arg("tf"), py::arg("y0"), py::arg("num_steps"), py::arg("exactSol"),
            R"(Compute the empirical order of convergence
            Parameters:
                t0 (float): Initial time
                tf (float): Final time
                y0 (list or numpy.array): Initial condition
                num_steps (int): Number of steps to  (it has to be a positive integer
                exactSol (function): Exact solution of the differential problem
                
            Returns:
                double: Empirical order of convergence
            )")
        .def_property_readonly("timeSteps", &ExplicitODESolver::getTimeSteps, "Return the time steps vector")
        .def_property_readonly("solSteps", &ExplicitODESolver::getSolSteps, "Return the solution steps vector")
        .def_property_readonly("errSteps", &ExplicitODESolver::getErrSteps, "Return the error steps vector");

    py::class_<ForwardEuler, ExplicitODESolver>(m, "ForwardEuler")
        .def(py::init<const std::function<Vector(double, const Vector&)>&>(),
            py::arg("df"),
            R"(Constructor for the ForwardEuler class
            
            Parameters:
                df (function): Function to compute the derivative of the solution
            )");

    py::class_<RK4, ExplicitODESolver>(m, "RK4")
        .def(py::init<const std::function<Vector(double, const Vector&)>&>(),
            py::arg("df"),
            R"(Constructor for the RK4 class
            
            Parameters:
                df (function): Function to compute the derivative of the solution
            )");

    py::class_<MidPoint, ExplicitODESolver>(m, "MidPoint")
        .def(py::init<const std::function<Vector(double, const Vector&)>&>(),
            py::arg("df"),
            R"(Constructor for the MidPoint class
            
            Parameters:
                df (function): Function to compute the derivative of the solution
            )");
}