#---------------------------------------------------------------------------------------------------------------
# Import libraries
import dataframe as df
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from dataframe import *
from itertools import product
#---------------------------------------------------------------------------------------------------------------
#Alternative constructor that takes as input path and separator
@classmethod
def from_csv(cls, filepath, sep=','):
    '''
    Alternative constructor that takes as input filepath and default separator.

    Parameters:
        - filepath: str, path to the csv file
        - sep: str, separator used in the csv file (default!)
    Returns:
        - dataframe: Dataframe, dataframe created from the csv file
    '''
    new_df=cls()
    new_df.import_csv(filepath,sep)
    return new_df
#---------------------------------------------------------------------------------------------------------------
#Alternative constructor that takes as input a pandas dfr
@classmethod
def from_pandas(cls, pd_df):
    '''
    Create a custom DataFrame from a Pandas DataFrame.

    Parameters:
        - pandas_df (pandas.DataFrame): Pandas DataFrame to convert.

    Returns:
        - Dataframe: Custom DataFrame created from the Pandas DataFrame.
    '''
    new_df=cls()
    # We need to prealloc the number of rows, since addColumn() asserts len(input_values)=nrows
    new_df.preallocDims(nrows= pd_df.shape[0], ncols= 0) 
    for col in pd_df.columns:
        values=pd_df[col].apply(lambda x: x if pd.notna(x) else None).tolist()
        new_df.addColumn(col,values)
    return new_df
#---------------------------------------------------------------------------------------------------------------
def to_pandas(self):
    '''
    Convert the custom DataFrame to a Pandas DataFrame.

    Returns:
        - pandas.DataFrame: Pandas DataFrame representation of the custom DataFrame.
    '''
    pandas_data = {}
    for column_name, column_values in self.getData().items():
        pandas_data[column_name] = column_values
        
    return pd.DataFrame(pandas_data)
#---------------------------------------------------------------------------------------------------------------
def fillNaN(self, attribute, newvalue, inplace=False):
        '''
        Fill missing values in the DataFrame with a specified value.

        Parameters:
            - value: The value to fill missing values with.
            - inplace (bool): If True, fill NaN values in-place. If False, return a new DataFrame.

        Returns:
            - Dataframe or None: A new DataFrame with missing values filled or None if inplace=True.
        '''
        if inplace:
            # !!!!!!!!!!!!!!!!!!!!!  FATTA MOLTO MALE !!!!!!!!!!!!!!!!!!!!!!!!
            # updated_col = [newvalue if pd.isna(val) else val for val in self[attribute]]
            # # drop it and add it updated (NB: we need absolutely to do a setter for cols and rows in C++)
            # self.dropColumn(attribute)
            # self.addColumn(attribute,updated_col)
            # return 
            self[attribute]= [newvalue if pd.isna(val) else val for val in self[attribute]]
        else:
            new_df = self.copy()
            new_df.fillNaN(attribute, newvalue, inplace=True)
            return new_df
#---------------------------------------------------------------------------------------------------------------

def covMatrix(self, attributes):
    '''
    Compute the covariance matrix between a list/tuple of attributes.

    Parameters:
        - attributes: list of str, attributes for which to construct the covariance matrix
    Returns:
        - covMatrix: numpy.ndarray, covariance matrix
    '''
    cov_matrix = np.zeros((len(attributes), len(attributes)))

    index_pairs = list(product(range(len(attributes)), repeat=2))
    for i,j in index_pairs:
        cov_matrix[i, j] = self.computeCov(attributes[i], attributes[j])
    return cov_matrix
#---------------------------------------------------------------------------------------------------------------
def corrMatrix(self, attributes):
    '''
    Compute the correlation matrix between a list/tuple of attributes.

    Parameters:
        - attributes: list of str, attributes for which to construct the correlation matrix
    Returns:
        - corrMatrix: numpy.ndarray, correlation matrix
    '''
    corr_matrix = np.zeros((len(attributes), len(attributes)))

    index_pairs = list(product(range(len(attributes)), repeat=2))
    for i,j in index_pairs:
        corr_matrix[i, j] = self.computeCorr(attributes[i], attributes[j])
    return corr_matrix
#---------------------------------------------------------------------------------------------------------------
def scatterplot(self, X, Y, group_by=None, xlabel=None, ylabel=None,title=None,palette=None):
    '''
    Create a scatterplot of two variables.

    Parameters:
        - X: str, name of the variable for the x-axis
        - Y: str, name of the variable for the y-axis
        - group_by: str, optional grouping variable for different colors in the plot
        - xlabel: str, label for the x-axis
        - ylabel: str, label for the y-axis
        - title: str, title of the plot
        - palette: str or list, color palette for different groups
    '''
    
    x_values = np.array(self[X])
    y_values = np.array(self[Y])
    hue = None if group_by is None else np.array(self[group_by])
    
    sns.scatterplot(x=self[X], y=self[Y], hue=hue,palette=palette)
    plt.xlabel(X if xlabel is None else xlabel)
    plt.ylabel(Y if ylabel is None else ylabel)
    plt.title(title if title is not None else f'{X} vs {Y} scatterplot')
    # Additional parameters when hue is not None
    if hue is not None:
        plt.legend(title=group_by)

#---------------------------------------------------------------------------------------------------------------
def hist(self, attribute, stat='count', bins='auto',color="lightblue", xlabel=None, title=None):
    '''
    Create a histogram of a variable.

    Parameters:
        - attribute: str, name of the variable for the histogram
        - stat: str, type of statistic to compute (count, frequency, probability,percent, density)
        - bins: int or str, number of bins or method to calculate bins
        - color: str, color of the histogram bars
        - x_label: str, label for the x-axis
        - title: str, title of the plot
    '''
        
    sns.histplot(np.array(self[attribute]), stat=stat, bins=bins,color=color)
    plt.xlabel(attribute if xlabel is None else xlabel)
    plt.ylabel("Count" if stat == 'count' else stat)
    plt.title(title if title is not None else f'Histogram of {attribute}')
#---------------------------------------------------------------------------------------------------------------
def boxplot(self, x=None, y=None, hue=None, xlabel=None,ylabel=None, title=None, palette=None,color=None):
    '''
    Create a boxplot of one or two variables.

    Parameters:
        - x: str, name of the variable for the x-axis
        - y: str, name of the variable for the y-axis
        - xlabel: str, label for the x-axis
        - ylabel: str, label for the y-axis
        - title: str, title of the plot
        - palette: str or list, color palette for different groups
        - color: str, color of the boxplot elements
    '''
    

    x_values = np.array(self[x]) if x is not None else None
    y_values = np.array(self[y]) if y is not None else None
    hue_value = np.array(self[hue]) if hue is not None else None
    sns.boxplot(x=x_values, y=y_values, hue= hue_value, palette=palette,color=color)
    plt.xlabel(x if xlabel is None else xlabel)
    plt.ylabel(y if ylabel is None else ylabel)
    plt.title(title if title is not None else f'Boxplot of {x}')
#---------------------------------------------------------------------------------------------------------------  
def heatmap(self, attributes, title=None, annot=True, cmap='coolwarm',triangular=None):
    '''
    Create a heatmap of the covariance matrix between attributes.

    Parameters:
        - attributes: list of str, attributes for which to construct the covariance matrix
        - title: str, title of the plot
        - annot: bool, whether to annotate each cell with the numeric value
        - cmap: str, colormap to use
        - triangular: str, 'lower', 'upper', or None to specify which triangular part to plot
    '''
    
    correlation_matrix = self.corrMatrix(attributes)
    # Create a mask to plot only the lower or upper triangular part
    mask = None
    if triangular == 'lower':
        mask = np.triu(np.ones_like(correlation_matrix), k=1)
    elif triangular == 'upper':
        mask = np.tril(np.ones_like(correlation_matrix), k=-1)
    # Plot the heatmap using Seaborn
    fmt = '.2f'
    sns.heatmap(correlation_matrix, annot=annot, cmap=cmap,fmt=fmt,
                xticklabels=attributes, yticklabels=attributes, mask=mask)
    plt.xticks(fontsize=9, rotation=45, ha='right')
    plt.yticks(fontsize=9)
    plt.title(title if title is not None else 'Covariance Matrix Heatmap')
#---------------------------------------------------------------------------------------------------------------
def scale(self, attribute,method='standard'):
    '''
    Scale the values of an attribute.
    
    Parameters:
        - attribute: str, name of the attribute to scale
        - method: str, type of scaling to perform (standard, min-max)
    '''
    if method not in ['standard', 'min-max']:
        raise ValueError(f'Invalid type for method: {method}')
    else:
        if method == 'standard':
            mu= np.repeat(self.computeMean(attribute),self.getDims()[0])
            sd= np.repeat(self.computeSd(attribute),self.getDims()[0])
            self[attribute] = ((np.array(self[attribute])- mu)/sd).tolist()
        elif method == 'min-max':
            m= np.repeat(self.computeMin(attribute),self.getDims()[0])
            M= np.repeat(self.computeMax(attribute),self.getDims()[0])
            self[attribute]= ((np.array(self[attribute])- m)/(M-m)).tolist()
    
    
    
#---------------------------------------------------------------------------------------------------------------
# Add new methods to Dataframe class
Dataframe.from_csv = from_csv
Dataframe.from_pandas = from_pandas
Dataframe.to_pandas = to_pandas
Dataframe.fillNaN = fillNaN
Dataframe.covMatrix = covMatrix
Dataframe.corrMatrix = corrMatrix
Dataframe.scatterplot = scatterplot
Dataframe.hist = hist
Dataframe.boxplot = boxplot
Dataframe.heatmap = heatmap
Dataframe.scale = scale
#---------------------------------------------------------------------------------------------------------------