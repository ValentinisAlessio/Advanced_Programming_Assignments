# Advanced Programming (HW01): Fantuzzi Giulio - Valentinis Alessio

# Table of contents
1. [About the assignment](#about-the-assignment)
    - [COO format](#coo-format)
    - [CSR format](#csr-format)
2. [About the code](#about-the-code)
    - [Files organization](#files-organization)
    - [Class methods, operators and free functions](#class-methods-operators-and-free-functions)

        -[Class methods](#class-methods)

        -[Operators](#operators)

        -[Free functions](#free-functions)
3. [How to compile](#how-to-compile)
4. [External resources for reference and learning](#external-resources-for-reference-and-learning)
5. [Workload division](#workload-division)

# About the assignment

This project was meant to implement a sparse matrix class. More precisely, we implemented and validated the formats:

- *Coordinate (**COO**)*
- *Compressed Sparse Row (**CSR**)* 

### COO format
The matrix can be stored using three arrays of length *nnz* (number of non-zeros):

- An array `values` containing all the nonzero values.
- An array `rows` of integers containing their corresponding row indices.
- An array `cols` of integers containing their corresponding column indices.

### CSR format

Again, the matrix can be stored using three arrays:

- An array `values` containing all the nonzero values (length *nnz*),
- An array `columns` of integers containing their column indices (length *nnz*),
- An array `row_idx` of integers with the cumulative number of nonzero entries up to the $i$-th row (excluded). The length of `row_idx` is $m+1$, being $m$ the number of rows of the matrix. By convention, we assume that `row_idx[0] = 0`.
  
**Note**: the quantity *row_idx[i+1] - row_idx[i]* represents the number of nonzero elements in the $i$-th row.

# About the code

## Files organization

The current folder contains:

- `main.cpp`: a *cpp* script which contains all the necessary code to test the correctness of the implemented classes;
- `build.sh`: a *bash* script for compilation (click [here](#how-to-compile) for more information about how to compile) 
- `include/`: this folder contains the following header files:

    - `SparseMatrix.hpp` provides the general scheme of *SparseMatrix, SparseMatrixCOO* and *SparseMatrixCSR* classes;

        **Note:** as implementation choice we decided to add matrix dimensions as input attributes of our classes' objects.
    - `SparseMatrix.tpl.hpp`, provides definition of classes' constructors, operators and methods;
    - `helper.hpp` provides some helper functions, which we implemented in order to make other class methods easier both to implement and to understand.


## Class methods, operators and free functions

### Class methods

General methods of a *SparseMatrix* (*COO* and *CSR* inherits them):

- `get_nrows()`: method to get the number of rows of the matrix;
- `get_ncols()`: method to get the number of columns of the matrix;
- `get_nzeros()`: method to get the number of nonzero values in the matrix;
- `get_values()`: method to get the *values* of the matrix (nonzero elements) ;
- `get_cols()`: method to get the *cols* vector of the matrix (column coordinates of nonzero elements);

Virtual methods (overridden in derived classes):

- `print()`: method to print the matrix in a convenient way. For matrices in which either the number of rows or the number of columns is >10, we decided to print just the sparse values;
- `get_info()`: method to print all information about the matrix (instead of calling other methods separately)
- Some helper functions:

    1. `findIndex()`: to find the index (in values vector) of an element at position (i,j) of the matrix;
    2. `getValue()`: to get the value at position (i, j) of the matrix;
    3. `setValue`: to set a new value at position (i, j) of the matrix;


Methods just for *SparseMatrixCOO*:

- `get_rows()`: method to get the *rows* vector of the matrix

Methods just for *SparseMatrixCOO*:

- `get_rows_idx()`: method to get the *rows_idx* vector of the matrix

### Operators
- operator `=` : as assignment operator;
- operator `*` : to compute matrix-vector product;
- operator `()` : to access and write matrix entry 

    *Note*: to better manage this operator we decided tu implement a `ProxySparse` class.

### Free functions
- `print_vector`: a templated function to print vectors in a convenient way;
- `COO_to_CSR()`: a function to convert a SparseMatrixCOO to a SparseMatrixCSR;
- `CSR_to_COO()`: a function to convert a SparseMatrixCOO to a SparseMatrixCSR;

**Important note:** `COO_to_CSR()` and `CSR_to_COO()` requires their input matrix to be allocated dynamically, 
in order to manage its deallocation during the conversion phase. This function could also be defined in a "static" way (input
deallocation would happen only at the end of the main), but our choice was to "delete the past" once for all.

# How to compile
To compile the `main.cpp` file, we provided a `build.sh`.

You should first give execution permission to the `build.sh`, and then execute it:
```bash
chmod +x build.sh
./build.sh
```
Alternatively, you might also execute directly the `build.sh` file as:
```bash
sh build.sh
```
or
```bash
bash build.sh
```
If the compilation was successful (it should be), the following message will appear on your terminal:
```bash
Build successful! You can run the program using ./sparse_matrix
```
Now you are ready to run the program by typing:
```bash
./sparse_matrix
```

# External resources for reference and learning

1. For a better general understanding:

    - https://stackoverflow.com/questions/23583975/convert-coo-to-csr-format-in-c
    - https://www.geeksforgeeks.org/sparse-matrix-representation/

2. For proxy implementation:
    - https://stackoverflow.com/questions/994488/what-is-proxy-class-in-c
    - https://github.com/pcafrica/advanced_programming_2023-2024/blob/main/exercises/03/examples/op_overload_proxy.cpp

3. For templatization part: https://cplusplus.com/doc/tutorial/functions2/


# Workload division
The vast majority of the work, above all the code structure part, has been done working together in presence.
However, to develop the hardest parts, we divided the work likely as follows:
- **Giulio**: Proxy class implementation, formatting the output, CSR to COO conversion, helper methods for COO; 
- **Alessio**: Templatization part, COO to CSR conversion, division of the code, helper methods for CSR.