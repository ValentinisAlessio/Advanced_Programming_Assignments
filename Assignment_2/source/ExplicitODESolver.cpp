#include "ExplicitODESolver.hpp"

void ExplicitODESolver::solve(const double &t0, const double& tf, const Vector& y0, const uint numsteps){
    // Check validity of input arguments
    // If I have no initial solution, throw an error
    if (y0.size() == 0) {
        throw std::runtime_error("y0 vector cannot be empty");
    }

    //Clear the vectors if they are not empty
    timeStep.clear();
    solStep.clear();

    // Apply the generic solving algorithm
    //Calculate the integration step
    double h = (tf - t0) / numsteps;
    //Insert the initial values inside the solution vectors
    timeStep.push_back(t0);
    solStep.push_back(y0);

    double t = t0;
    Vector y = y0;

    //Retrieve the size of the algorithm in order to allocate a matrix of proper dimension
    uint s = b.size();
    //Allocate the matrix of weights for practicality
    Matrix K = Matrix::Zero(y0.size(), s);

    for (uint i = 0; i < numsteps; ++i) {
        //Initialize the first column
        K.col(0) = func(t, y);

        for (uint j = 1; j < s; ++j) {
            Vector sum = K.leftCols(j) * A.row(j).head(j).transpose();
            // Methods of Eigen:
            //K.leftCols(j): returns the first j columns of the matrix K
            //A.row(j).head(j).transpose(): selects the first j elements of the row with index j and gets the transpose

            //Define the j-th columns of the K matrix
            K.col(j) = func(t + (c[j] * h), y + (h * sum));
        }

        //Update solution vector and time
        y += h * K * b;
        t += h;

        //Insert the new values inside the proper vectors
        timeStep.push_back(t);
        solStep.push_back(y);
    }
}

void ExplicitODESolver::writeCSV(const std::string& filename) const {
    // Check coherence of the attributes
    if (timeStep.size() != solStep.size()) {
        throw std::invalid_argument("Mismatched sizes of timeStep and solStep vectors");
    }

    //Opening a CSV file for writing the solutions
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Write header
    file << "t";
    for (uint i = 0; i < solStep[0].size(); ++i) {
        file << ",y" << i + 1;
    }
    file << std::endl;

    // Write data
    for (uint i = 0; i < timeStep.size(); ++i) {
        file << timeStep[i];
        for (uint j = 0; j < solStep[i].size(); ++j) {
            file << "," << solStep[i][j];
        }
        file << std::endl;
    }

    //Close the file
    file.close();
}

// Compute the maximum error using the infinity norm
double ExplicitODESolver::computeInfinityNormError(const std::function<Vector(double)>& exactSol) const {
    // Compute the maximum error using the infinity norm
    double maxError = 0.0;

    for (size_t i = 0; i < timeStep.size(); ++i) {
        // Calculate exact and numerical solutions
        Vector exact = exactSol(timeStep[i]);
        Vector numerical = solStep[i];
        Vector error = (exact - numerical).cwiseAbs(); // element-wise Absolute error(function of Eigen)
        double currentMax = error.maxCoeff(); // Max absolute value in the vector (function of Eigen)

        // Adjust the maximum error only if i find a value greater that the one memorized
        if (currentMax > maxError) {
            maxError = currentMax;
        }
    }

    return maxError;
}

// Function to calculate the mean execution time over 100 runs
double ExplicitODESolver::meanExeTime(const double &t0, const double &tf, const Vector& y0, const uint &numSteps){
    std::vector<double> durations;  //Initialize a vector for storing the durations
    for (uint i=0; i<100; ++i){
        // Compute the execution time for each iteration
        auto start_time = std::chrono::high_resolution_clock::now();
        solve(t0, tf, y0, numSteps);
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds> (end_time - start_time);
        durations.push_back(duration.count());      //For each iteration of the test insert the time of execution in the vector
    }
    // Return the mean of the vector durations
    return std::accumulate(durations.begin(), durations.end(), 0.0)/durations.size();
}

//Function to calculate the single execution time of the solving algorithm
double ExplicitODESolver::singleExeTime(const double &t0, const double &tf, const Vector& y0, const uint &numSteps){
    // Compute the time taken to solve the algorithm
    auto start_time = std::chrono::high_resolution_clock::now();
    solve(t0, tf, y0, numSteps);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds> (end_time - start_time);

    // Return the actual value in microseconds
    return duration.count(); 
}

// Function to compute the empirical order of convergence
double ExplicitODESolver::computeEmpiricalOrder(const double &t0, const double &tf, const Vector &y0, const uint &numSteps, const std::function<Vector(double)>& exactSol){
    // Try a first solve
    solve(t0, tf, y0, numSteps);
    double error_prev = computeInfinityNormError(exactSol);

    // Decrease stepsize by a factor of ten and recompute the error
    uint newNumSteps = 10*numSteps;
    solve(t0, tf, y0, newNumSteps);
    double error_curr = computeInfinityNormError(exactSol);

    // Calculate empirical order of convergence
    double order = std::log10(error_prev/error_curr);
    
    return order;
}

//Different solvers depend only on Buthcer tableau
ForwardEuler::ForwardEuler(const std::function<Vector(double, const Vector&)>& f):
        ExplicitODESolver(Matrix(1, 1), Vector(1), Vector(1), f) {
            A << 0.;
            c << 0.;
            b << 1.;
        }

RK4::RK4(const std::function<Vector(double, const Vector&)>& f):
        ExplicitODESolver(Matrix(4, 4), Vector(4), Vector(4), f) { 
            A << 0. ,0. ,0. ,0.,
                 1./2., 0., 0., 0.,
                 0., 1./2., 0., 0.,
                 0., 0., 1., 0.;
            c << 0., 1./2., 1./2., 1.;
            b << 1./6., 1./3., 1./3., 1./6.;
        }

MidPoint::MidPoint(const std::function<Vector(double, const Vector&)>& f):
        ExplicitODESolver(Matrix(2, 2), Vector(2), Vector(2), f) {
            A << 0., 0.,
                 1./2., 0.;
            c << 0., 1./2.;
            b << 0., 1.;
        }
