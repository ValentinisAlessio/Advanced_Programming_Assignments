# Advanced Programming HW3
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
│   ├── 📄 main_math.ipynb
│   └── 📄 main_stat.ipynb
│ 
├── 📂 bindings/ 
│   ├── 📄 dataframe_bindings.cpp
│   └── 📄 ODE_bindings.cpp
│
├── 📂 datasets/
│   └── 📊 housing.csv
│
├── 📂 include/
│   ├── 📄 ExplicitODESolver.hpp	
│   └── 📄 dataframe.hpp
│
├── 📂 python_modules/
│   ├── 📂 DataFrame/
│   │   ├── 📄 DFModule.py	
│   │   └── 📄 __init__.py
│   ├── 📂 ODE/
│   │   ├── 📄 ODEModule.py	
│   │   └── 📄 __init__.py
│   └── 📄 __init__.py
│
├── 📂 solutions/
│
├── 📂 source/
│   ├── 📄 ExplicitODESolver.cpp
│   └── 📄 dataframe.cpp
│
├── 📂 unit_testing/
│   ├── 📄 df_unittesting.py
│   └── 📄 ode_unittesting.py
│
├── 📒 .gitignore
├── 📝 CMakeLists.txt
├── 📰 README.md
└── 📄 setup.py
```

More details about the files are available [here](#module-a-files-organization) (for module A) and [here](#module-d-files-organization) (for module D).

# How to build

<u>**OUR TWO MODULES TAKE FUNCTIONALITIES FROM THIRD PARTY LIBRARIES !**</u>

In particular:

- **Module A:** <u>*GSL-GNU Scientific Library* (version 2.7.1)</u>, since it provided an easy and friendly way to compute various statistics;
- **Module D:** <u>*Eigen* (version 3.4.0)</u>, since it provided an easier way to implement matrices-like objects and perform very quickly linear algebra.

If you don't have such libraries installed on your computer, click [here](#step-by-step-guide-to-install-third-party-libraries). Instead, if you already have them installed, you can directly skip [here](#manual-installation-via-cmake).

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
$ wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
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

## Manual installation via CMake

We provided a `CMakeLists.txt` to facilitate the compilation of our modules. From the root, you need to perform the following steps:

Create a `build/` folder and move inside it:
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

### Flags to specify libraries position
Our `CMakeLists.txt` files provides some flags to allow the user to specify, if desired, the explicit path where to find the required third party libraries. This might be useful especially for all those users who already have the libraries, but not at the path `/opt/include`. 

Such flags are:

- `PYBIND_DIR` for pybind11 library;
- `GSL_DIR` for gsl library;
- `EIGEN3_DIR` for Eigen library;

and can be used in the build process with:
```bash
cmake -DGSL_DIR=/path/to/gsl-2.7.1 -DEIGEN3_DIR=/path/to/eigen-3.4.0 -DPYBIND_DIR=/path/to/pybind11 ..
```
To test the modules in a Python script, a preliminary step is necessary:
```bash
export PYTHONPATH=/path/to/build/
```
**Note**: if you run such command from the current position (build directory), just do `export PYTHONPATH=$PWD`.

## Automatic installation via `pip`

Alternatively, this package provides also a `setup.py` script, allowing for automatic installation of the toolbox via pip, the Python package manager.

To start the automatic installation, just run
```bash
python<version> setup.py build
```
to build the package underneath `build/` directory, or
```bash
python<version> setup.py install
```
to install the package.
Or more directly 
```bash
python<version> setup.py build_ext
```

**SINGLE LINE INSTALLATION** : if you want to spare some keybord presses, just perform:
```bash
pip<version> install .
```

### Set of CMake flags

To set the CMake flags specified above also for your `setup.py` installation, just set them like environmental variables, like
```bash
export GSL_DIR=path/to/GSL
export EIGEN3_DIR=path/to/Eig3
export PYBIND_DIR=path/to/pybind11
```
An alternative may be to set the environmental variable `CMAKE_ARGS`
```bash
export CMAKE_ARGS="-DGSL_DIR=/path/to/gsl -DEIGEN3_DIR=/path/to/eigen3 -DPYBIND_DIR=/path/to/pybind11"
pip<version> install .
(or python<version> setup.py build_ext)
```

<u>*Anyway we recommend to let CMake look for the libraries authomatically*</u>

# Module A: Statistics module

This module is meant to perform statistical analyses on data imported from a CSV file. We selected the [California Housing Prices](https://www.kaggle.com/datasets/camnugent/california-housing-prices) dataset from Kaggle, and used it to test all the functionalities implemented. 

## Module A: about the code
### Module A: files organization

The codes regarding this module are contained in the following subfolders:
- `apps/`: this folder contains `main_stat.ipynb`, a *python notebook* with all the necessary code to import the dataset, compute statistical analyses, and perform some tests on the binded module and on the new functionalitites;
- `include/`: this folder contains `dataframe.hpp`, an header file containing the declaration of the class *Dataframe* and the signature of its methods
- `source/`: this folder contains `dataframe.cpp`, a *cpp* script containing the definitions of all *Dataframe*'s methods, besides some helper functions, which we developed in order to make other class methods easier both to implement and to understand
- `bindings/`: this folder contains `dataframe_bindings.cpp`, a *cpp* script that contains the code necessary to bind the *cpp* code to *python*.

### Module A: class and methods bindings

Dataframe class and all its methods have been bound in Python with the use of `pybind11`. In order to work with our class in more Python-friendly way (and to simplify everything in enhancing the class), some additional bindings have been added.

**Note:** to make things easier we also added some new methods directly in the cpp class.

- `__repr__`: to have a human-readable summary when printing a Dataframe object (distinguishing empy dataframes with filled ones)

- `__getitem__`: to access a Dataframe elements. More precisely, we provided 4 ways of getting items:

    ```cpp
    // (1) Python access of type df[row_idx, attribute]
    .def("__getitem__", [](const Dataframe &d, const std::pair<unsigned int,std::string> &idx) {
        return d.getData()[idx.second][idx.first-1];
    })
    // (2) Python access of type df[row_idxs list/tuple,attribute]
    .def("__getitem__", [](const Dataframe &d, const std::pair<std::vector<unsigned int>,std::string> &idx) {
        std::vector<ColumnValue> result;
        for (auto i : idx.first) {
            result.push_back(d.getData()[idx.second][i-1]); 
        }
        return result;
    })
    // (3) access df[attribute] (NB: it is the same as getColumn(), but will be more user-friendly in Python)
    .def("__getitem__", [](const Dataframe &d, const std::string &attribute) {
        return d.getColumn(attribute);
    })
    // (4) access df[rowIdx] (NB: it is the same as getRowByIdx(), but will be more user-friendly in Python)
    .def("__getitem__", [](const Dataframe &d, const unsigned int &idx) {
        return d.getRowByIdx(idx); 
    })
    ```

- `__setitem__`: to set Dataframe elements. More precisely, we provided 3 ways of setting items:
    ```cpp
    // (1) Binding for setting a value in a Dataframe ENTRY
    .def("__setitem__", [](Dataframe &d, const std::pair<unsigned int, std::string> &idx, const ColumnValue &value) {
        d.setDfEntry(idx, value); 
    })
    // (2) Binding for changing an entire COLUMN of a Dataframe
    .def("__setitem__", [](Dataframe &d, const std::string &attribute, const std::vector<ColumnValue> &column) {
        d.setDfColumn(attribute, column);
    })
    // (3) Binding for changing an entire ROW of a Dataframe
    .def("__setitem__", [](Dataframe &d, const unsigned int &idx, const std::vector<ColumnValue> &row) {
        d.setDfRow(idx, row);
    })
    ```

- `copy`: to get a copy of a Dataframe object (useful to develop a `fillNaN` method )

- `preallocDims`: to pre-allocate dimensions of an empty dataset. It was needed to implement a conversion from Pandas dataframe, since our class always assert dimensions correctness when adding rows or columns (and we didn't want to implement a new variant for already existing functionalities!)


### Module A: Python new funcionalities

In the `python_modules/DataFrame/` folder, a `DFModule.py` is provided, where some interesting new functionalities has been provided:

- **Alternative constructors**
    - `from_csv` : alternative constructor that takes as input filepath and default separator;
    - `from_pandas`: to create a custom DataFrame from a Pandas DataFrame;
- **Conversion to Pandas**
    - `to_pandas` : to convert a custom DataFrame to a Pandas DataFrame
- **Data visualization**
    - `scatterplot` : create a scatterplot of two variables of the Dataframe
    - `hist` : create a histogram of a variable of the Dataframe
    - `boxplot` : create a boxplot of one or two variables
    - `heatmap` : create a heatmap of the covariance matrix between a list/tuple of attributes


- **Other functionalities**
    - `fillNaN` : to fill missing values in the DataFrame with a specified value (with an *inplace* option)
    - `covMatrix`: compute the covariance matrix (as *numpy.ndarray*) between a list/tuple of attributes
    - `corrMatrix` : compute the correlation matrix (as *numpy.ndarray*) between a list/tuple of attributes
    - `scale` : scale the values of an attribute (both *min-max* and *standard* scaling)

It is possible to access to the documentation of all these methods directly from Python with:
```python
help(function_name)
```
All the functionalities of this module have been tested, and they are showed in the Jupyter notebook located in `apps/main_stat.ipynb`.

# Module D: ODE module

This module contains numerical solvers for Ordinary Differential Equations (ODE).
A general formulation of this problem is:

$$\begin{cases}
\frac{\mathrm{d}\mathbf{y}}{\mathrm{d}t} = \mathbf{f}(t, \mathbf{y})\\
y(t_0) = y_0
\end{cases}$$

where $\mathbf{y}\in \mathbb{R}^N, \mathbf{f} : \mathbb{R} \times \mathbb{R}^N \rightarrow \mathbb{R}^N$.

The aim was to implement explicit Runge Kutta Solvers, such as Forward Euler, RK4 and Midpoint.

### Module D: files organization

The codes regarding this module are aswell contained in the following subfolders:
- `apps/`: this folder contains the following scripts for testing the classes implemented
    - `main_math.ipynb`: a *python* script which contains all the necessary code to test the correctness of the implemented classes;
- `include/`: this folder contains the following header files:
    - `ExplicitODESolver.hpp` provides the general scheme of *ExplicitODESolver, ForwardEuler, RK4* and *MidPoint* classes;
- `source/`: this folder contains the following source files;
    - `ExplicitODESolver.cpp` containing the definitions of classes' constructors and methods;
- `bindings/`: this folder contains *cpp* scritps to bind module's functionalities;
- `python_modules/ODE/`: this folder contains a *python* script that expands module's functionalities, providing some more methods to plot solutions, errors and trajectories, along with a decorator that allows to measure the average time taken from a function over 100 runs.

### Module D: classes and methods bindings

Every class and module existing from the last Homework has been bound in Python with the use of `pybind11`. The only methods added were wrappers for the getter methods `getTimeSteps`, `getSolSteps`, `getErrorSteps` in order to make them read-only attributes.
```cpp
.def_property_readonly("timeSteps", &ExplicitODESolver::getTimeSteps, "Return the time steps vector")
.def_property_readonly("solSteps", &ExplicitODESolver::getSolSteps, "Return the solution steps vector")
.def_property_readonly("errSteps", &ExplicitODESolver::getErrSteps, "Return the error steps vector")
```

### Module D: other methods

In the `python_modules/ODE/` folder, an `ODEModule.py` is provided, and some other methods of the parent class `ExplicitODESolver` class are defined.

- `@time_execution`: decorator that measures the average time taken from a function to run over over 100 attempts;
- `method()`: free function that calls directly the correct constructor when passed as input;
- `chrono_solve`: method that measures in Python the time taken from the method solve to run, thanks to the decorator `@time_execution`
- `plotSolution`: method that using Matplotlib prints the plot of the solution vector against the time vector.
- `plotError`: method that plots the error against the time in infinity norm
- `plotLogError`: same as above, just in log scale over the `y` axis
- `plotTrajectory`: method that allows to plot the trajectory of the differential problem, i.e. the components of the solution vector

All the functionalities of this module have been tested, and they are showed in the Jupyter notebook located in apps/main_math.ipynb.

# Unit testing

We used the `unittesting` library of Python to check automatically the validity of the results of our cpp modules. In particular we tested some of their functionalities against the most known python libraries written for similar purposes, for example `numpy` for checking dataframe's statistic methods and `scipy` for checking ODEs's results.

To run those scripts, just move into `unit_testing/` folder and run
```bash
python<version> df_unittesting.py
python<version> ode_unittesting.py
```
**Note** : in order to display the full test procedure, method by method, you have the possibility to add the flag `-v` at the end of the command above. More precisely:

```bash
python<version> df_unittesting.py -v
python<version> ode_unittesting.py -v
```

# Workload division and external references

The vast majority of the work has been done working together in presence or in videocall. However, to develop the hardest parts, we divided the work likely as follows:

- **Giulio:** general focus on Statistic module, integration of bindings and added new modules, unit testing of the modules.
- **Alessio:** general focus on ODEs module, integration of bindings and added new modules, CMake and setup.py implementation.

External resources for reference and learning:

- Unit testing:
    - https://machinelearningmastery.com/a-gentle-introduction-to-unit-testing-in-python/
    - https://www.browserstack.com/guide/unit-testing-python
- Pybind11:
    - https://pybind11.readthedocs.io/en/stable/
- Scipy for ODE:
    - https://docs.scipy.org/doc/scipy/tutorial/integrate.html
