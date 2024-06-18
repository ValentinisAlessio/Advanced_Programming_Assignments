#include <iostream>
#include "Functions.hpp"
#include "ExplicitODESolver.hpp"

int main(){
    try{
        // Initial conditions and parameters
        double t0 = 0.0;
        double tf = 10.0;

        std::function<Vector(const double&, const Vector&)> df;
        std::function<Vector(const double&)> f;
        int steps;
        Vector y0;
        
        int choice_run;
        std::cout << "Choose the type of analysis you want to perform (1 for 100 runs or 2 for a single run): ";
        std::cin >> choice_run;

        // Throw error if invalid input provided
        if(choice_run != 1 && choice_run !=2){
            throw std::invalid_argument("Insert a valid option between 1 and 2!");
        }

        std::cout << "Choose the number of steps: ";
        std::cin >> steps;

        // Throw error if invalid input provided
        if (steps <= 0) {
            throw std::invalid_argument("Number of steps must be positive!");
        }

        uint numSteps = (uint)steps;

        y0 = Vector(2);
        y0 << 1.0, 0.0; // Initial values of y
        //Choose the differential problem
        df =df2;
        f=f2;
        /* !!IF YOU WANT TO CHANGE EXAMPLE FROM THE ARMONIC OSCILLATOR TO THE EXPONENTIAL FUNCTION,
          JUST COMMENT THE PREVIOUS TWO LINES AND UNCOMMENT THE FOLLOWING TWO LINES!! */
        
        // df=df1;
        // f=f1;

        ForwardEuler euler(df);
        RK4 rk4(df);
        MidPoint midp(df);

        double eulerDuration;
        double rk4Duration;
        double midpDuration;

        switch (choice_run){
            case 1:
                eulerDuration = euler.meanExeTime(t0, tf, y0, numSteps);
                rk4Duration = rk4.meanExeTime(t0, tf, y0, numSteps);
                midpDuration = midp.meanExeTime(t0, tf, y0, numSteps);
                break;
            case 2:
                eulerDuration = euler.singleExeTime(t0, tf, y0, numSteps);
                rk4Duration = rk4.singleExeTime(t0, tf, y0, numSteps);
                midpDuration = midp.singleExeTime(t0, tf, y0, numSteps);
                break;
        }

        //Redirect the output to an output.txt file
        std::string outDir="../output/output_math/";
        std::ofstream output(outDir+"output.txt");
        if (!output.is_open()) {
            std::cerr << "Whoops! Error in opening output file. Execution aborted!" << std::endl;
            return 1; // Return an error code
        }
        std::streambuf *originalCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(output.rdbuf());
        
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////// (1) FORWARD EULER TEST ///////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << "Forward Euler simulation" << std::endl;
        
        // Use the defined function `df` to solve the ODE using ForwardEuler
        if (choice_run == 1){
            std::cout << "Average duration of Forward Euler on 100 runs and "<<numSteps<<" steps: " 
                      << eulerDuration << " microseconds" << std::endl;
        }else{
            std::cout << "Duration of Forward Euler on a single run and "<< numSteps<<" steps: "
                      << eulerDuration << "microseconds" << std::endl;
        }
        //Print output to a CSV file
        euler.writeCSV(outDir+"euler_output.csv");

        //Computing maximum error
        double maxErrorEu = euler.computeInfinityNormError(f);
        std::cout << "Forward Euler Max error: " << maxErrorEu << std::endl;

        //Computing empurical order of convergence
        double orderEu = euler.computeEmpiricalOrder(t0,tf,y0,numSteps,f);
        std::cout << "Forward Euler empirical order of convergence: " << orderEu << std::endl;
        std::cout<<std::endl;


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////// (2) RK4 TEST //////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << "RK4 simulation" << std::endl;

        // Use the defined function `f` to solve the ODE using RK4
        if (choice_run == 1){
            std::cout << "Average duration of RK4 on 100 runs and "<<numSteps<<" steps: " 
                      << rk4Duration << " microseconds" << std::endl;
        }else{
            std::cout << "Duration of RK4 on a single run and "<< numSteps<<" steps: "
                      << rk4Duration << "microseconds" << std::endl;
        }

        //Print output to a CSV file
        rk4.writeCSV(outDir+"rk4_output.csv");

        //Computing maximum error
        double maxErrorRk = rk4.computeInfinityNormError(f);
        std::cout<< "RK4 Max error: "<<maxErrorRk<<std::endl;
        
        //Computing empurical order of convergence
        double orderRk = rk4.computeEmpiricalOrder(t0,tf,y0,numSteps,f);
        std::cout << "RK4 empirical order of convergence: " << orderRk << std::endl;
        std::cout<<std::endl;
        


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////// (3) MIDPOINT TEST //////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        std::cout << "MidPoint simulation" << std::endl;
        
        // Use the defined function `f` to solve the ODE using midpoints
        if (choice_run == 1){
            std::cout << "Average duration of Midpoint on 100 runs and "<<numSteps<<" steps: " 
                      << midpDuration << " microseconds" << std::endl;
        }else{
            std::cout << "Duration of Midpoint on a single run and "<< numSteps<<" steps: "
                      << midpDuration << "microseconds" << std::endl;
        }

        //Print output to a CSV file
        midp.writeCSV(outDir+"midpoint_output.csv");

        //Computing maximum error
        double maxErrorMp = midp.computeInfinityNormError(f);
        std::cout<< "Midpoint Max error: "<<maxErrorMp<<std::endl;

        //Computing empurical order of convergence
        double orderMp = midp.computeEmpiricalOrder(t0,tf,y0,numSteps,f);
        std::cout << "Midpoint empirical order of convergence: " << orderMp << std::endl;
        std::cout<<std::endl;

        //----------------------------------------------------------------
        //Clode the file and reset the std output buffer
        std::cout.rdbuf(originalCoutBuffer);
        //File will automatically close when gone out of scope
        std::cout<<"output.txt saved successfully in "<<outDir<<std::endl;

    }
    catch(const std::exception &e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

