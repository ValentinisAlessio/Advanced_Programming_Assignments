'''
About:
- this script is meant to compare the results of the ODE module with the results of the SciPy module
- to run this test, use python<your_version> ode_testing.py -v
'''


#DYNAMIC PATH SELECTION TO IMPORT THE MODULES
import sys
import os

# Get the absolute path of the current script
current_path = os.path.abspath(os.path.dirname(__file__))

# Construct the paths to the required directories
modules_path = os.path.join(current_path, '..')
ode_path = os.path.join(modules_path, 'ODE')

# Add the directories to the Python path if they're not already there
if modules_path not in sys.path:
    sys.path.append(modules_path)
if ode_path not in sys.path:
    sys.path.append(ode_path)
#---------------------------------------------------------------------------------------------------------------

import unittest
from python_modules.ODE import ODEModule as ODE
from scipy.integrate import solve_ivp
import numpy as np

# Test class
print('-'*70)
print("Testing ODE module")
print('-'*70)

class TestODE(unittest.TestCase):
    def setUp(self):
        # Set as test a simple ODE
        self.df = lambda t, y: -y
        def f(t):
            y = np.zeros(1)
            y[0] = np.exp(-t)
            return y
        self.f = f

    def test_solve_euler(self):
        solver = ODE.method("ForwardEuler", self.df)
        self.assertIsNone(solver.solve(0, 10, [1.0], 100))

    def test_solve_rk4(self):
        solver = ODE.method("RK4", self.df)
        self.assertIsNone(solver.solve(0, 10, [1.0], 100))

    def test_solve_midpoint(self):
        solver = ODE.method("MidPoint", self.df)
        self.assertIsNone(solver.solve(0, 10, [1.0], 100))

    def test_writeCSV(self):
        solver = ODE.method("ForwardEuler", self.df)
        solver.solve(0, 10, [1.0], 100)
        
        # Get the absolute path of the current script
        current_script_path = os.path.abspath(__file__)
        # Construct the path to the solutions folder
        solutions_folder = os.path.join(os.path.dirname(current_script_path), '../solutions')
        # Ensure the solutions folder exists or create it if it doesn't
        if not os.path.exists(solutions_folder):
            os.makedirs(solutions_folder)

        # Construct the absolute path to the CSV file within the solutions folder
        csv_path = os.path.join(solutions_folder, 'test.csv')

        # Write the CSV file using the absolute path
        solver.writeCSV(csv_path)

    def test_accuracy_euler(self):
        solver = ODE.method("ForwardEuler", self.df)
        solver.solve(0, 10, [1.0], 100)
        error = solver.computeInfinityNormError(self.f)
        self.assertAlmostEqual(error, 0, delta=1e-1)

    def test_accuracy_rk4(self):
        solver = ODE.method("RK4", self.df)
        solver.solve(0, 10, [1.0], 100)
        error = solver.computeInfinityNormError(self.f)
        self.assertAlmostEqual(error, 0, delta=1e-4)

    def test_accuracy_midpoint(self):
        solver = ODE.method("MidPoint", self.df)
        solver.solve(0, 10, [1.0], 100)
        error = solver.computeInfinityNormError(self.f)
        self.assertAlmostEqual(error, 0, delta=1e-2)

    def test_accuracy_against_scipy(self):
        # Create an instance of the solver
        my_solver = ODE.method("RK4", self.df)
        # Define a SciPy solver for comparison
        def scipy_solver(t, y):
            return solve_ivp(self.df, (t, t + 1), [y], method='RK45').y[0][-1]

        # Solve with our solver
        my_solver.solve(0, 1, [1], 100)
        my_result = my_solver.solSteps[-1]
        # Solve with SciPy
        scipy_result = scipy_solver(0, 1)

        # Add assertions to compare results
        self.assertTrue(np.allclose(my_result, scipy_result, atol=1e-3)) # Tolerance set to 1e-3 to account for the different step sizes


if __name__ == '__main__':
    unittest.main()
