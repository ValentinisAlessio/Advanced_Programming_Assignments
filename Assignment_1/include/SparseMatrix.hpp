// Header guards
#ifndef SPARSEMATRIX_HPP_
#define SPARSEMATRIX_HPP_
//---------------------------------------------------------------------------------------------------------------------
// Libraries
#include<iostream>
#include<vector>
#include<cassert>
//---------------------------------------------------------------------------------------------------------------------
// (1) SparseMatrix class declaration (base class)
template <typename T>
class SparseMatrix{
public:
    // Default constructor
    SparseMatrix(const unsigned int &nr,
                 const unsigned int &nc,
                 const std::vector<T> &d,
                 const std::vector<unsigned int> &c);
                 /*rows vector not defined here: its meaning will be completely different among derived classes*/
    // Copy constructor
    SparseMatrix(const SparseMatrix<T> &other);
    // Assignment operator
    SparseMatrix<T> & operator =(const SparseMatrix<T> &other);
    // Virtual destructor
    virtual ~SparseMatrix() {std::cout<<"Destructed SparseMatrix"<<std::endl;}
    /*Since we used std::vector, we don't need to worry about memory deallocation*/

    // Method to get the number of rows
    unsigned int get_nrows()const{return n_rows;}
    // Method to get the number of columns
    unsigned int get_ncols()const{return n_cols;}
    // Method to get the number of nonzero values
    unsigned int get_nzeros()const{return values.size();}
    // Method to get the nonzero values
    std::vector<T> get_values()const{return values;}
    // Method to get the column vector
    std::vector<unsigned int> get_cols()const{return cols;}
    // Virtual operator for matrix-vector product
    virtual const std::vector<T> operator*(const std::vector<T> &vec)const=0;
    // Virtual method to print a matrix in a convenient way
    virtual void print()=0;
    // Virtual method to print all information about the matrix
    virtual void get_info() const=0;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Proxy to implement reading-writing access on a matrix
    class ProxySparse {
    public:
        ProxySparse(SparseMatrix& matrix,
                    const unsigned int i,
                    const unsigned int j): matrix(matrix), row(i), col(j) {}

        // Reading operator
        operator T() const {
            return matrix.getValue(row, col);
        }
        // Writing operator
        ProxySparse& operator=(const T &value) {
            matrix.setValue(row, col, value);
            return *this;
        }
    private:
        SparseMatrix<T>& matrix;
        unsigned int row;
        unsigned int col;
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Access operator
    ProxySparse operator()(const unsigned int i, const unsigned int j) {
        // Check validity of coordinates (i,j)
        assert(i>=0 && j>=0 && i<n_rows && j<n_cols);
        // If valid coordinates, return!
        return ProxySparse(*this, i, j);
    }

protected:
    /* Some (virtual) helper functions to make other class methods easier both to 
       implement and understand (check "helper.hpp" file for their definition) */
    // (I) Helper function to find the index (in values vector) of an element at position (i,j)
    virtual const int findIndex(const unsigned int i, const unsigned int j) const=0;
    // (II) Helper function to get the value at position (i, j)
    virtual const T getValue(const unsigned int i, const unsigned int j) const=0;
    // (III) Helper function to set the new value at position (i, j)
    virtual void setValue(const unsigned int i, const unsigned int j, const T value)=0;
    
    // Attributes of the class SparseMatrix
    unsigned int n_rows;
    unsigned int n_cols;
    std::vector<T> values;
    std::vector<unsigned int> cols;
};

//---------------------------------------------------------------------------------------------------------------------
// (2) SparseMatrixCOO class declaration (derived class of SparseMatrix)
template <typename T>
class SparseMatrixCOO : public SparseMatrix<T>{
public:
    // Constructor
    SparseMatrixCOO(const unsigned int &nr,
                    const unsigned int &nc,
                    const std::vector<T> &d,
                    const std::vector<unsigned int> &c,
                    const std::vector<unsigned int> &r);
    // Copy constructor
    SparseMatrixCOO(const SparseMatrixCOO<T> &other);
    // Destructor
    ~SparseMatrixCOO() {std::cout<<"Destructed SparseMatrixCOO"<<std::endl;}
    //Assignment operator
    SparseMatrixCOO<T> & operator =(const SparseMatrixCOO<T> &other);
    // Method to print a SparseMatrixCOO
    void print() override;
    // Method to print information about a SparseMatrixCOO
    void get_info() const override;
    //Method to get the rows
    std::vector<unsigned int> get_rows()const{return rows;}
    // Operator for COO matrix-vector product
    const std::vector<T> operator*(const std::vector<T> &vec) const override;
    

private:
    /* Some helper functions to make other class methods easier both to 
       implement and understand (check "helper.hpp" file for their definition) */
    // (I) Helper function to find the index (in values vector) of an element at position (i,j)
    const int findIndex(const unsigned int i, const unsigned int j) const override;
    // (II) Helper function to get the value at position (i, j)
    const T getValue(const unsigned int i, const unsigned int j) const override;
    // (III) Helper function to set the new value at position (i, j)
    void setValue(const unsigned int i, const unsigned int j, const T value) override;

    //Here the only private attribute of the class SparseMatrixCOO (rows)
    std::vector<unsigned int> rows;
};

//---------------------------------------------------------------------------------------------------------------------
// (3) SparseMatrixCSR class declaration (derived class of SparseMatrix)
template <typename T>
class SparseMatrixCSR: public SparseMatrix<T>{
public:
    // Constructor
    SparseMatrixCSR(const unsigned int &nr,
                    const unsigned int &nc,
                    const std::vector<T> &d,
                    const std::vector<unsigned int> &c,
                    const std::vector<unsigned int> &r);
    
    // Copy constructor
    SparseMatrixCSR(const SparseMatrixCSR<T> &other);
    // Destructor
    ~SparseMatrixCSR() {std::cout<<"Destructed SparseMatrixCSR"<<std::endl;}
    // Assignment operator
    SparseMatrixCSR<T> & operator =(const SparseMatrixCSR<T> &other);
    // Method to print a SparseMatrixCSR
    void print() override;
    // Method to print information about a SparseMatrixCSR
    void get_info() const override;
    //Method to get the rows indexes
    std::vector<unsigned int> get_rows_idx()const{return rows_idx;}
    // Operator for CSR matrix-vector product
    const std::vector<T> operator*(const std::vector<T> &vec) const override;

private:
    /* Some helper functions to make other class methods easier both to 
       implement and understand (check "helper.hpp" file for their definition) */
    // (I) Helper function to find the index (in values vector) of an element at position (i,j)
    const int findIndex(const unsigned int i, const unsigned int j) const override;
    // (II) Helper function to get the value at position (i, j)
    const T getValue(const unsigned int i, const unsigned int j) const override;
    // (III) Helper function to set the new value at position (i, j)
    void setValue(const unsigned int i, const unsigned int j, const T value) override;

    //Here the only private attribute of the class SparseMatrixCSR (rows_idx)
    std::vector<unsigned int> rows_idx;
};

// Function to convert a SparseMatrixCOO into a SparseMatrixCSR
template <typename T>
SparseMatrixCSR<T>* COO_to_CSR(SparseMatrixCOO<T> *matrix);

// Function to convert a SparseMatrixCSR into a SparseMatrixCOO
template <typename T>
SparseMatrixCOO<T>* CSR_to_COO(SparseMatrixCSR<T> *matrix);
//---------------------------------------------------------------------------------------------------------------------
//Link to the definition file
#include "SparseMatrix.tpl.hpp"
#endif
