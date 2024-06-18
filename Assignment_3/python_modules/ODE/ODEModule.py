from ODE import ExplicitODESolver, ForwardEuler, RK4, MidPoint
import numpy as np
import time
import matplotlib.pyplot as plt

def time_execution(func):
    '''
    Decorator to time the execution of a function
    '''
    def wrapper(*args, **kwargs):
        l=[]
        for _ in range(100):
            start = time.time()
            result = func(*args, **kwargs)
            end = time.time()
            time_taken = (end - start) * 1e6
            l.append(time_taken)
        print(f"Function {func.__name__} took on average {np.mean(l)} microseconds to execute")
        return result
    return wrapper

# Function to call directly the ODE specific subclass
def method (sovler_type, df):
    '''
    Alternative constructor to call directly the ODE specific solver

    Parameters:
        - sovler_type: str, type of solver to use
        - df: function, function that defines the ODE

    Returns:
        - solver: ODE solver, class with the solver for the ODE
    '''
    if sovler_type == "ForwardEuler":
        return ForwardEuler(df)
    elif sovler_type == "RK4":
        return RK4(df)
    elif sovler_type == "MidPoint":
        return MidPoint(df)
    else:
        raise ValueError("Invalid solver type. Please choose from 'ForwardEuler', 'RK4', or 'MidPoint'")
#---------------------------------------------------------------------------------------------------------------
@time_execution
def chrono_solve(self, t0, tf, y0, num_steps):
    '''
    Solve the ODE keeping track of the time

    Parameters:
        - t0: float, initial time
        - tf: float, final time
        - y0: float, initial condition
        - num_steps: int, number of steps to take
    Returns:
        - time: float, time taken from the execution of the solver
    '''
    self.solve(t0, tf, y0, num_steps)
#---------------------------------------------------------------------------------------------------------------
def plotSolution(self):
    '''
    Plot the solution of the ODE

    Parameters:
        - None
    Returns:
        - Plot of the solution
    '''

    plt.plot(self.timeSteps, self.solSteps)
    plt.title("Solution plot")
    plt.xlabel("Time")
    plt.ylabel("Solution")    

#---------------------------------------------------------------------------------------------------------------
def plotError(self,sol):
    '''
    Plot the error of the ODE

    Parameters:
        - sol: function, exact solution of the ODE
    Returns:
        - Plot of the error
    '''
    self.computeInfinityNormError(sol)
    plt.plot(self.timeSteps, self.errSteps)
    plt.title("Error plot")
    plt.xlabel("Time")
    plt.ylabel("Error (in infinity norm)")

#---------------------------------------------------------------------------------------------------------------
def plotLogError(self, sol):
    '''
    Plot the error of the ODE in log scale

    Parameters:
        - sol: function, exact solution of the ODE
    Returns:
        - Plot of the error in log scale
    '''
    self.computeInfinityNormError(sol)
    plt.plot(self.timeSteps, self.errSteps)
    plt.title("Error plot in log scale")
    plt.xlabel("Time")
    plt.ylabel("Error (in infinity norm)")
    plt.yscale("log")

#---------------------------------------------------------------------------------------------------------------
def plotTrajectory(self):
    '''
    Plot the trajectory of the ODE

    Parameters:
        - None
    Returns:
        - Plot of the trajectory
    '''

    # Convert the solution steps to a numpy array
    solSteps = np.vstack(self.solSteps)

    if len(solSteps) == 0:
        print("No solution steps to plot.")
        return
    
    if solSteps.shape[1] < 2:
        print("Solution dimensionality is insufficient for plotting a trajectory.")
        return
    
    dimensions = solSteps.shape[1]
    for dim in range(dimensions - 1):
        plt.plot(solSteps[:, dim], solSteps[:, dim + 1])
    
    plt.xlabel("y0")
    plt.ylabel("y1")
    plt.title("Trajectory plot")


#---------------------------------------------------------------------------------------------------------------
# Add new methods to the ODE class
ExplicitODESolver.chrono_solve = chrono_solve
ExplicitODESolver.plotSolution = plotSolution
ExplicitODESolver.plotError = plotError
ExplicitODESolver.plotLogError = plotLogError
ExplicitODESolver.plotTrajectory = plotTrajectory
#---------------------------------------------------------------------------------------------------------------