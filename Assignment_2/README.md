# Advanced Programming HW2
Fantuzzi Giulio (giulio.fantuzzi01@gmail.com), Valentinis Alessio (ale.vale1705@gmail.com)

# Table of contents

1. [Project structure](#project-structure)
2. [How to build](#how-to-build)
3. [Module A: Statistics](#module-a-statistics-module)
4. [Module D: ODE](#module-d-ode-module)
5. [Workload division and references](#workload-division-and-external-references)

# Project structure

```
📂 project/
│ 
├── 📂 apps/
│   ├── 📝 CMakeLists.txt
│   └── 📄 main_math.cpp
│   └── 📄 main_stats.cpp
│ 
├── 📂 datasets/
│   └── 📊 housing.csv
│
├── 📂 include/
│   └── 📄 ExplicitODESolver.hpp
│   └── 📄 Functions.hpp	
│   └── 📄 dataframe.hpp
│
├── 📂 source/
│   ├── 📝 CMakeLists.txt
│   └── 📄 ExplicitODESolver.cpp
│   └── 📄 Functions.cpp
│   └── 📄 dataframe.cpp
│
├── 📒 .gitignore
├── 📝 CMakeLists.txt
└── 📰 README.md
```
More details about the files are available [here](#module-a-files-organization) (for module A) and [here](#module-d-files-organization) (for module D).

# How to build

<u>**OUR TWO MODULES TAKE FUNCTIONALITIES FROM THIRD PARTY LIBRARIES !**</u>

In particular:

- **Module A:** <u>*GSL-GNU Scientific Library* (version 2.7.1)</u>, since it provided an easy and friendly way to compute various statistics;
- **Module D:** <u>*Eigen* (version 3.4.0)</u>, since it provided an easier way to implement matrices-like objects and perform very quickly linear algebra.

If you don't have such libraries installed on your computer, click [here](#step-by-step-guide-to-install-third-party-libraries). Instead, if you already have them installed, you can directly skip [here](#step-by-step-guide-with-cmake).

## Step by step guide to install third party libraries

This short tutorial is meant to guide the user to install correctly the libraries needed by our modules.

**IMPORTANT NOTE**: as a convention, we will install them into the `/opt/` directory. 

### GSL 2.7.1 installation

Open your terminal and download the library with the following command:

```bash
wget https://ftp.gnu.org/gnu/gsl/gsl-2.7.1.tar.gz
```

The downloaded library will appear as a zipped folder. To extract the archive, type:
```bash
tar -xzvf gsl-2.7.1.tar.gz
```

Navigate to the extracted directory:
```bash
cd gsl-2.7.1
```

Configure and Install:
```bash
./configure --prefix=/opt
sudo make
sudo make install
```
**Note:** The `--prefix=/opt` option ensures that GSL is installed in the `/opt` directory.

After installation, you can check the installation correctness by looking at the contents of the /opt directory:
```bash
ls /opt
```
You should see directories like `bin`, `include`, `lib`, as well as possibly a `gsl` directory at the path `/opt/include/` .

You can verify that the correct version of GSL is installed by running:
```bash
/opt/bin/gsl-config --version
```
If everything went successfully, the output should be *2.7.1* .

### Eigen 3.4.0 installation

Open your terminal and download the library with the following command:

```bash
$ wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip

```

The downloaded library will appear as a zipped folder. To extract the archive, type:
```bash
tar -xzvf eigen-3.4.0.tar.gz
```

Navigate to the extracted directory:
```bash
cd eigen-3.4.0
```
Configure and Install:
```bash
mkdir build
cd build
sudo cmake .. -DCMAKE_INSTALL_PREFIX=/opt
sudo make
sudo make install
```
**Note:** the `DCMAKE_INSTALL_PREFIX=/opt option` ensures that Eigen is installed in the `/opt` directory. If everything went successfully, you should expect to have the folder `eigen3/` at the path `/opt/include/` .

## Step by step guide with CMake

We provided some `CMakeLists.txt` files in order to make the building process easier for the user. From the root, the main steps to do are the following:

Create a folder `build/` and move inside it:
```bash
mkdir build && cd build
```

### Default build
If third party libraries are installed into your `/opt/include`, and if you want to build both the modules, just run:

```bash
cmake ..
```
**Note:** after this command you will notice that a folder named `output` will appear in your project root. Inside it, you will find two subfolders:

- `output_math/`: in which the outputs of the math module will be saved ;
- `output_stats/`: in which the outputs of the statistics module will be saved ;

Everything is now set up to compile the executable files. Proceed by running the following command:
```bash
make -j<N>
```
This will compile both the main scripts and will generate two executable files: `main_stats` and `main_math` (both stored in `/build/apps/`). To run them, follow the instructions suggested [here](#module-a-how-to-run-main_stats) (for `main_stats`) and [here](#module-d-how-to-run-main_math) (for `main_math`).

### Flags to specify libraries position
Our `CMakeLists.txt` files provides some flags to allow the user to specify, if desired, the explicit path where to find the required third party libraries. This might be useful especially for all those users who already have the libraries, but not at the path `/opt/include`. 

Such flags are:

- `GSL_DIR` for gsl library;
- `EIG_DIR` for Eigen library;

and can be used in the build process with:
```bash
cmake -DGSL_DIR=/path/to/gsl-2.7.1 -DGSL_DIR=/path/to/eigen-3.4.0 ..
```

### Cache variables to build modules separately

Our `CMakeLists.txt` files also allow each of the modules to be compilable as a standalone shared library, allowing an independent use. To manage this aspect, we defined two specific cache variables, which the user should specify to enable modules according to preferences.

Such cache variables are:

- `STATS` to enable compilation of the statistics module;
- `MATH` to enable compilation of the ODEs module;

and as default, they are set to ON (so modules will be both compiled).

For example, if the user wants to compile <u>only the stats module</u>, he should type:

```bash
cmake -DMATH=OFF ..
```
**Note:** to compile only the math module, instead, set `-DSTATS=OFF`.

Let's now consider an example to give a better understanding of these cache variables. Assume that a user compiled only the stats module (setting `-DMATH=OFF` in the cmake call), and later on he wants to compile <u>only the math module</u>. The correct way to do it should be:

```bash
cmake -DMATH=ON -DSTATS=OFF ..
```

**ATTENTION:** after each call, both `STATS` and `MATH` would be set to the value specified in the previous call. For instance, if the user (after the two calls in our example) updates the modules and wants to build them again, he would need to explicitely reset both of them to ON!



# Module A: Statistics module

This module is meant to perform statistical analyses on data imported from a CSV file. We selected the [California Housing Prices](https://www.kaggle.com/datasets/camnugent/california-housing-prices) dataset from Kaggle, and used it to test all the functionalities implemented. 

## Module A table of contents
- [Module A: about the code](#module-a-about-the-code)
    - [Module A: files organization](#module-a-files-organization)
    - [Module A: class attributes](#module-a-class-attributes)
    - [Module A: class methods and helper functions](#module-a-class-methods-and-helper-functions)
        - [Dataframe general methods](#dataframe-general-methods)
        - [Dataframe statistics methods](#dataframe-statistics-methods)
        - [Dataframe helper methods and free functions](#dataframe-helper-methods-and-free-functions)
    - [Dataframe Iterators](#dataframe-iterators)
- [Module A: how to run main_stats](#module-a-how-to-run-main_stats)

## Module A: about the code
### Module A: files organization

The codes regarding this module are contained in the following subfolders:
- `apps/`: this folder contains `main_stat.cpp`, a *cpp* script with all the necessary code to import the dataset, compute statistical analyses, and save them into an external file in the folder `output/output_stats`;
- `include/`: this folder contains `dataframe.hpp`, an header file containing the declaration of the class *Dataframe* and the signature of its methods
- `source/`: this folder contains `dataframe.cpp`, a *cpp* script containing the definitions of all *Dataframe*'s methods, besides some helper functions, which we developed in order to make other class methods easier both to implement and to understand

### Module A: class attributes
 Our *Dataframe* class provides methods to import csv files, compute basic statistics on data and deal with Nan and missing values. To manage correctly different data types, and eventual missing values/NaN, we decided to exploit `std::variant` and `std::optional` of the STL. More precisely, we decided to make our *Dataframe*'s values belonging to a *ColumnValue* type, which we defined as:

```cpp
using ColumnValue = std::optional< std::variant<double,std::string> >;
```
Firstly, we decided to provide our *Dataframe* class with two attributes regarding the dimensions of the dataframe:

```cpp 
unsigned int nrows; // number of rows of the dataframe
unsigned int ncols; // number of columns of the dataframe
```
*NB: such attributes are not const since a dataframe might be modified (hence, its dimensions might change)*

To store the data, instead, our choice was to use a *standard map* as data structure. The general idea was to store each dataframe's column as a *key-value* "pair", in which the key represents the column name, while the value is actually a vector of *ColumnValue*, containing the elements of such column

```cpp 
std::map< std::string, std::vector<ColumnValue> > dataset;
```
Some notes:
- as explained before, the non-constness of such attribute is due to the possibility of modifying a *Dataframe* object
- our first approach was based on using a *std::unordered_map*, since the order of the columns of a dataframe, in general, is not relevant. However, the (alphanumeric) order of the keys provided by an ordered map, made the implementation of several methods much easier (*for example, when inserting a new row to the dataframe*). <u>We know that unordered maps might be less efficient in terms of performances</u>, but we think that for this project it was still convenient to use ordered maps!


### Module A: class methods and helper functions
#### Dataframe general methods

**IMPORTANT NOTE**: for all those const methods supposed to print something, our implementation choice was to avoid raising exceptions. Instead of stopping the execution of the program (like for non-const methods), we thought it was more appropriate to just print an error message to the user and continuing with the execution!


List of *Dataframe*'s **CONST** methods

- `colnames()` : method to get columns'names as a *std::vector\<std::string\>*;

- `getDims()` : method to get dataframe's dimensions as a *std::pair*, whose first element contains the number of rows, while the second one contains the number of columns (attributes);

- `getData()` : method to get the data of the *Dataframe* as a *std::map*

- `getColumn()` : method that receives as input a column name and returns the values contained in such column ( as a *std::vector\<ColumnValue\>* );
- `getRowByIdx()` : method that receives as input a row index and returns such row ( as a *std::vector\<ColumnValue\>* );
- `printRowByIdx()` : method that receives as input a row index and print it to terminal;
- `printRow()` : method that receives as input a row ( as *std::vector\<ColumnValue\>* ) and print it;
- `printColumn()` : method that receives as input a column name and print its values
- `head()` : method that prints to terminal the first *n* rows of the dataframe, with *n* received as input parameter. If *n* is greater than the number of rows, all the rows will be printed;
- `tail()` : method that prints to terminal the last *n* rows of the dataframe, with *n* received as input parameter. If *n* is greater than the number of rows, again, all the rows will be printed;
- `countNaN()` : method that counts the number of NaN/missing values of a given column
- `filterRows()` : method that allow to filter rows basing on an attribute condition. Our implementation choice was to make this function return a *Dataframe*, in order to allow concatenated filterings (see the example in the main);

    

List of *Dataframe*'s **NON-CONST** methods

- `import_csv()` : method to import a csv file, that receives as input the filepath of the file and allows the user to specify a separator *sep* (with a default option set to comma ','). Notice that such method can be called explicitly by the user, but it is also called implicitly by the class constructor;
- `setHeader()` : method to set the header for an empty Dataframe ;
- `addRow()` : method to add a row at a certain index (received as input)
- `addColumn()` : method to add a column at a certain index. If the input column name already exists, the function will be called recursively and it will insert a new column with name *"attribute.copy"* ;
- `dropRowByIdx()` : method that receives a row index as input and drops such row; 
- `dropColumn()` : method that receives a column name as input and drops such column ; 
- `dropNaN()` : method that drops from the dataframe all the rows containing at least one Nan/missing value ;

#### Dataframe statistics methods
List of *Dataframe*'s methods to compute statistics:

- `computeSum()` : method that receives as input a (numerical) column name and returns the sum of its values ;
- `computeMean()` : method that receives as input a (numerical) column name and returns the mean of its values ;
- `computeMin()` : method that receives as input a (numerical) column name and returns the minimum among its values ;
- `computeMax()` : method that receives as input a (numerical) column name and returns the maximum among its values ;
- `computeMedian()` : method that receives as input a (numerical) column name and returns the median of its values ;
- `computePercentile()` : method that receives as inputs a (numerical) column name and a percentile $p \in [0,100]$, and returns the *p-th* percentile among its values ;
- `computeVariance()` : method that receives as input a (numerical) column name and returns the variance of its values ;
- `computeSd()` : method that receives as input a (numerical) column name and returns the standard deviation of its values ;
- `computeCov()` : method that receives as input two (numerical) columns' names and returns their covariance ;
- `computeCorr()` : method that receives as input two (numerical) columns' names and returns their correlation (in terms of Pearson's correlation coefficient $\rho$ );
- `covMat()` : method that receives as input a vector of (numerical) columns and print their covariance matrix
- `corrMat()` : method that receives as input a vector of (numerical) columns and print their correlation matrix
- `table()` : method that receives as input a column name and print the frequency table of its values. Such function, in principle, can be applied to both numerical and categorical columns, but it is more meaningful if applid to categorical ones (like in our main) ;
- `summary()` : method that receives as input a (numerical) column name and compute several statistics (*min, 1st quantile, median, 3rd quantile, max, mean, variance and std. dev*), printing them in a concise and convenient way;

**NOTE** : obviously all these methods, except from *table()*, make sense if applied to numerical columns! 

#### Dataframe helper methods and free functions
List of *Dataframe*'s helper methods:

- `numColumnToStdVector()` : helper method to convert a Column into a std::vector
- `invalidAttributeName()` : helper method that spots invalid attribute in inputs
- `invalidRowIdx()` :  helper method that spots invalid row index in inputs
- `anyInvalidAttribute()` : helper method that spots if a *std::vector* of attribute names contains any invalid attribute


List of other "free" helper functions:

- `ColumnValueToString()` : helper method to convert a *ColumnValue* into a *std::string*
- `ColumnValueVectorToString()` : helper method to convert a *std::vector\<ColumnValue\>* into a *std::vector\<std::string\>*

### Dataframe Iterators
Our *Dataframe* class provides iterable objects. Since data are stored in a *std::map<std::string, std::vector\<ColumnValue\>>*, it is pretty easy to iterate over dataframe columns. For example, if we consider an object *df* belonging to *Dataframe* class, we just need to do something like:
```cpp 
for(const auto col : df.getData()){
    std::string attribute= col.first;
    std::vector<ColumnValue> values= col.second;
    // ...do something...
}
```
On the contrary, iterating row-by-row was not (originally) possible. Since row-by-row iteration was something useful to explot in other methods, we decided to declare a `RowIterator` class inside the *Dataframe* class. Thanks to such solution, we can iterate over rows simply by doing:
```cpp
// -----------------------------------------
// (1) if RowIterator used outside the class
for(const auto& row : df){
    // ...do something...
}
// -----------------------------------------
// (2) if RowIterator used inside the class
for(const auto& row : *this){
    // ...do something...
}
// -----------------------------------------
```

## Module A: how to run main_stats
Once the compilation phase with *CMake* (explained [here](#step-by-step-guide-with-cmake) ) has been completed , an executable `main_stats` will appear in the folder `apps/`.

First of all, let's move inside `apps/`
```bash
cd apps/
```

To run the executable, two arguments <u>must</u> be provided:

1) the name of the csv dataset to parse (stored in the folder `datasets/`)
2) the name of the output file where to store the results (it will be saved into `../output/output_stats/`)

**IMPORTANT:** the user is required to pass as input just the files' names, NOT their filepath !

The very general command to type is the following:

```bash
./main_stats dataset.csv results.txt
```

For example, to test the executable with respect to the choosen dataset, the command will be:
```bash
./main_stats housing.csv housing_stats.txt
```

If everything is successfully, the following message will be printed to terminal:

```
Data imported successfully!
... analyzing housing.csv ....
housing_stats.txt saved successfully in ../output/output_stats/
```


 **Important note**: the  `output/` folder inside `build/` has been set as a symbolic link directory to the root, so the files contained in such folder will be stored permanently in the root even if the `build/` directory will be deleted in future.


# Module D: ODE module

This module contains numerical solvers for Ordinary Differential Equations (ODE).
A general formulation of this problem is:

$$\begin{cases}
\frac{\mathrm{d}\mathbf{y}}{\mathrm{d}t} = \mathbf{f}(t, \mathbf{y})\\
y(t_0) = y_0
\end{cases}$$

where $\mathbf{y}\in \mathbb{R}^N, \mathbf{f} : \mathbb{R} \times \mathbb{R}^N \rightarrow \mathbb{R}^N$.

The aim was to implement explicit Runge Kutta Solvers, such as Forward Euler, RK4 and Midpoint.

## Module D: table of contents
- [About Runge Kutta Methods](#about-runge-kutta-methods)
- [Module D: about the code](#module-d-about-the-code)
    - [Module D: files organization](#module-d-files-organization)
    - [Module D: class methods](#module-d-class-methods)
    - [Module D: examples provided](#module-d-examples-provided)
    - [Module D: guide to the custom set of the problem](#module-d-guide-to-the-custom-set-of-the-problem)
- [Module D: how to run main_math](#module-d-how-to-run-main_math)
- [Accuracy, efficiency and order of convergence of the methods implemented](#accuracy-efficiency-and-order-of-convergence-of-the-methods-implemented)
## About Runge Kutta Methods
[Runge Kutta methods](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods) are numerical methods to solve ODE problems, more specifically Cauchy problems. They require a Butcher tableau, which general form is:

$$
\begin{array}{c|cccc}
	c_1 & a_{11} & a_{12} & \dots & a_{1s}\\
	\vdots & \vdots & \vdots & \ddots &\vdots\\
	c_s & a_{s1} & a_{s2} & \dots &a_{s,s}\\
	\hline
	& b_1 & b_2 &\dots & b_s\\
\end{array}
$$

The purpose of the assignment was to implement explicit Runge-Kutta algorithms, so a special case in which the A matrix of the tableau is inferiorly triangular.

The general algorithm would be:
$$y_{n+1} = y_n + h_{n+1} \sum_{i=1}^{s} b_i K_i$$
$$K_i := f(t_n+c_i h_{n+1}, y_n + h_{n+1} \sum_{j=1}^{i-1} a_{ij} K_j)$$


## Module D: about the code
### Module D: files organization

The codes regarding this module are aswell contained in the following subfolders:
- `apps/`: this folder contains the following scripts for testing the classes implemented
    - `main_math.cpp`: a *cpp* script which contains all the necessary code to test the correctness of the implemented classes;
- `include/`: this folder contains the following header files:

    - `ExplicitODESolver.hpp` provides the general scheme of *ExplicitODESolver, ForwardEuler, RK4* and *MidPoint* classes;

        **Note:** as implementation choice we decided to provide hardcoded examples of differential problems, providing also their exact solution, in order to assess accuracy and convergence of the method.
    - `Functions.hpp` provides the declaration of the two provided ODE functions.
- `source/`: this folder contains the following source files;
    
    - `ExplicitODESolver.cpp` containing the definitions of classes' constructors and methods;
    - `Functions.cpp` containing the definitions of functions declared in the respective *.hpp* file.

### Module D: class Methods
The module is implemented in a way to have a generic solver (`ExplicitODESolver` class) which, taken as attributes the Butcher Tableau of the solver, it computes the numerical solutions, storing the results into two `std::vector`, one for the solutions and one for the times.

- `getTimeSteps()` and `getSolSteps()`: getter methods that allow to access the protected attributes of the class.
- `solve()`: method to compute all the steps of the *times* and solutions and store them into two attributes of the class, called `timeStep` and `solStep`.
- `writeCSV()`: method to write the times and solutions into a separate `CSV` file, with header columns `t, y1, y2, ..., yN`.
- `computeInfinityNormError()`: method to compute the infinity norm of the solution vector with respect to the exact solution.
- `meanExeTime()`: method to test the solve algorithm accuracy and efficiency on average on 100 runs. 
- `singleExeTime()`: method to test the solve algorithm accuracy and efficiency on a single run.
- `computeEmpiricalOrder()`: method to compute the empirical order of convergence of the solver. It evaluates the error with two number of steps and calculates the order of convergence by definition.

The derived classes `ForwardEuler`, `RK4`, and `MidPoint` implement only the constructor, which implement the corresponding Butcher Tableau.
To emphasize this approach we opted for run-time polymorphism, through inheritance, in order to implement a single solver algorithm in the base class. In this way, the derived classes should only initialize the attributes corresponding to the butcher tableau, without the need to further define the `solve()` method. 
Furthermore, while compile-time polymorphism might offer performance advantages and potentially better optimizations, the current code structure and the nature of the problem domain (where solvers are fixed and the variability lies in the test problems) align well with the use of run-time polymorphism. This also allowed us to define a generic solver that needs to be personalized only by specific constructors.

### Module D: examples provided
In the file `Functions.cpp` are defined two basic examples to showcase the functionality of the solvers. They are:
- `df1`: $$ \begin{cases} \frac{dy}{dt} = -y\\ y_0 = 0 \end{cases}; $$
- `f1`: Exact solution of the `df1` problem $$ y(t) = e^{-t}; $$
- `df2`: $$\begin{cases}\frac{\mathrm{d}\mathbf{y}}{\mathrm{d}t} = \begin{bmatrix} y_1 \\ -y_0 \end{bmatrix} \\ y_0 = \begin{bmatrix}1\\0\end{bmatrix}\end{cases};$$
- `f2`: Exact solution of the `df2` problem $$ \mathbf{y} = \begin{bmatrix}\cos(t)\\ -\sin(t) \end{bmatrix};$$

All these problems are fixed to be solved in the interval $[0,10]$.

### Module D: guide to the custom set of the problem
If you want to provide a custom ODE problem to solve, follow the following steps:
- Modify into the `Functions.cpp` file the definition of the problem
    ```cpp
    Vector df3(const double& t, const Vector& y) {
        //Input arguments are set to void because it's possible not to use both of them
        (void)y;
        (void)t;
        // Example: Define a simple system of ODEs
        Vector dydt(N);     // N shall be the dimension of the problem
        dydt[0] = ...;   // dy0/dt = ...
        dydt[1] = ...;  // dy1/dt  = ...
        ...
        dydt[N] = ...;  // dyN/dt = ...
        return dydt;
    }
    ```
- Into the `main_math.cpp` file provide the proper interval of integration an initial solution (namely on lines 8-9 and 23-24)
    ```cpp
    int main(){
        ...
        // Initial conditions and parameters
        double t0 = 0.0;        // line 8
        double tf = 10.0;
        ...
        y0 = Vector(2);         // line 23
        y0 << 1.0, 0.0; // Initial values of y
        //Choose the differential problem
        df =df3;
        f=f3;
        ...
    }
    ```

## Module D: how to run main_math

Once the compilation phase with *CMake* (explained [here](#step-by-step-guide-with-cmake) ) has been completed , an executable `main_math` will appear in the folder `apps/`.

First of all, let's move inside `apps/`
```bash
cd apps/
```

To run the executable, just type:
```bash
./main_math
```
**Note:** even if no argument needs to be provided from terminal, the user will be required to specify some input parameters in an interactive way!

Our repository's code is based on the provided example `df2`, but it can be easily changed by the user (uncommenting lines 31-32 and commenting lines 26-27 on the `main_math.cpp`).

### Inputs
As previously explained, some input will be required. In particular:
1.  ```
    Choose the type of analysis you want to perform (1 for 100 runs or 2 for a single run):
    ```
    Suggestions:

    - Insert 1 for a test on 100 runs for average efficiency;
    - Insert 2 for a test on a single run to fastly showcase accuracy;

2.  ```
    Choose the number of steps:
    ```
    Insert the number of steps you want your problem to be solved on.

### Outputs

Four output files will be generated into the `output/output_math/` directory:
- three `<method>_solutions.csv` files, containing the solution of the differential problem for each of the solvers ;
- one `output.txt` file containing, for each method, the duration of the `solve()` method in order to assess efficiency, the maximum error of the method (in infinity norm) and the empirical order of convergence.

**Important note**: as explained for the statistics module, the  `output/` folder inside `build/` has been set as a symbolic link directory to the root, so the files contained in such folder will be stored permanently in the root even if the `build/` directory will be deleted in future.

## Accuracy, efficiency and order of convergence of the methods implemented
Remember that the methods implemented are **Forward Euler, RK4, Explicit Midpoints**. For the following evaluations, the second example provided is used, kepping fixed at $100$ the number of steps.
To showcase the accuracy of the method, the maximum infinity norm of the error is used. With this approach, we can appreciate how the Euler method is the worst in accuracy, followed by midpoint method, and lastly by RK4, which is the one that provides the best accuracy.

For what concerns efficiency, the time of execution is assessed. On a 100 run, without any kind of optimization, we can see how the Euler method is the fastest solver, while the worst is the RK4 method. So we can see that, even if RK4 is by far the most accurate method, it requires the longest time to be computed.

To assess order of convergence, instead, we have to run the algorithm with different number of steps (implying a different length of integration step). First of all we have to define what the order of convergence is.

**def**: A method is said to be convergent in $[t_0,t_f]$ if, given $h=\max_{1 \leq n \leq N} h_n$, $\lim_{h \to 0^+} \max_{1 \leq n \leq N} ||e_n|| =0$. A Method is said to be convergent of order $p$ if $f \in C^p$ and $\max_{1 \leq n \leq N} e_n = O(h^p)$.

Given this definition, we can evaluate the empirical order of convergence by computing the infinity error increasing the number of steps by a factor of 10 (so reducing the integration step by the same factor), an see by which power of 10 the infinity error of the method is decreased.
We can see by running the example 2 with starting number of steps $100$, that the empirical orders of convergence result:
- Euler: $1.09968$;
- RK4: $4.00342$;
- Midpoint: $2.00288$;
 
These results coincide with theoretical previsions of order of convergence of these solvers (i.e. $1$ for Euler, $2$ for Midpoint and $4$ for RK4).


# Workload division and external references

The vast majority of the work has been done working together in presence or in videocall. However, to develop the hardest parts, we divided the work likely as follows:

- **Giulio:** general focus on Statistic module, integration of gsl external library to our class, unit testing of the modules
- **Alessio:** general focus on ODEs module, integration of Eigen external library to our class, CMakes implementation

External resources for reference and learning:

1. External libraries documentation:
    - https://www.gnu.org/software/gsl/#documentation
    - https://eigen.tuxfamily.org/dox/
2. CMake:
    - https://cmake.org/cmake/help/latest/guide/tutorial/index.html
3. Redirecting output to external files
    - https://stackoverflow.com/questions/4810516/c-redirecting-stdout
4. Dealing with std::variant
    - https://en.cppreference.com/w/cpp/utility/variant
    - https://levelup.gitconnected.com/understanding-std-visit-in-c-a-type-safe-way-to-traverse-variant-objects-dbeff9b47003