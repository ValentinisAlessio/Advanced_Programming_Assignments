'''
About:
- this script is meant to compare the results of the dataframe module with the results of the numpy module
- to run this test, use python<your_version> df_unittesting.py -v
'''

# DYNAMIC PATH SELECTION TO IMPORT THE MODULES
import sys
import os

# Get the absolute path of the current script
current_path = os.path.abspath(os.path.dirname(__file__))

# Construct the paths to the required directories
modules_path = os.path.join(current_path, '..')
df_path = os.path.join(modules_path, 'DataFrame')

# Add the directories to the Python path if they're not already there
if modules_path not in sys.path:
    sys.path.append(modules_path)
if df_path not in sys.path:
    sys.path.append(df_path)
#---------------------------------------------------------------------------------------------------------------

# Libraries
from python_modules.DataFrame import DFModule as df
import numpy as np
import pandas as pd
import unittest
from itertools import combinations
 
# Dataframe
d=df.Dataframe()
csv_filename = os.path.join(current_path, '..', 'datasets', 'housing.csv')
d.import_csv(csv_filename)
d.dropNaN()
num_attributes = ["households", "housing_median_age", "latitude",
                  "longitude", "median_house_value","median_income",
                  "population", "total_bedrooms", "total_rooms"]

# Test class
print('-'*70)
print("Testing dataframe module")
print('-'*70)

class StatTests(unittest.TestCase):
    def test_sum(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeSum(attribute)),
                                   float(np.sum(np.array(d.getColumn(attribute)))),
                                   delta=1e-5, msg="Test on computeSum() failed on " + attribute)
    def test_mean(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeMean(attribute)),
                                   float(np.mean(np.array(d.getColumn(attribute)))),
                                   delta=1e-5, msg="Test on computeMean() failed on " + attribute)
    def test_min(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeMin(attribute)),
                                   float(np.min(np.array(d.getColumn(attribute)))),
                                   delta=1e-5, msg="Test on computeMin() failed on " + attribute)
            
    def test_max(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeMax(attribute)),
                                   float(np.max(np.array(d.getColumn(attribute)))),
                                   delta=1e-5, msg="Test on computeMax() failed on " + attribute)
    def test_25percentile(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computePercentile(attribute,0.25)),
                                   float(np.percentile(np.array(d.getColumn(attribute)),0.25)),
                                   delta=1e-5, msg="Test on computePercentile() failed on " + attribute)    
    def test_median(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeMedian(attribute)),
                                   float(np.median(np.array(d.getColumn(attribute)))),
                                   delta=1e-5, msg="Test on computeMedian() failed on " + attribute)   
    def test_variance(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeVariance(attribute)),
                                   float(np.var(np.array(d.getColumn(attribute))) * len(np.array(d.getColumn(attribute))) / (len(np.array(d.getColumn(attribute))) - 1)),
                                   delta=1e-5, msg="Test on computeVariance() failed on " + attribute)
    def test_sd(self):
        for attribute in num_attributes:
            self.assertAlmostEqual(float(d.computeSd(attribute)),
                                   float(np.std(np.array(d.getColumn(attribute)), ddof=1)),
                                   delta=1e-5, msg="Test on computeVariance() failed on " + attribute)
    def test_cov(self):
        for attribute1,attribute2 in list(combinations(num_attributes, 2)):
            self.assertAlmostEqual(float(d.computeCov(attribute1,attribute2)),
                                        float(np.cov(np.array(d.getColumn(attribute1)),np.array(d.getColumn(attribute2)),ddof=1)[0][1]),
                                        delta=1e-5, msg="Test on computeCov() failed on " + attribute1 + " and " + attribute2)
    def test_corr(self):
        for attribute1,attribute2 in list(combinations(num_attributes, 2)):
            self.assertAlmostEqual(float(d.computeCorr(attribute1,attribute2)),
                                        float(np.corrcoef(np.array(d.getColumn(attribute1)),np.array(d.getColumn(attribute2)))[0][1]),
                                        delta=1e-5, msg="Test on computeCorr() failed on " + attribute1 + " and " + attribute2)
if __name__ == '__main__':
    # begin the unittest.main()
    unittest.main()
