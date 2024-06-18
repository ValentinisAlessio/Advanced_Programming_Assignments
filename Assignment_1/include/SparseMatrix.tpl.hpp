//---------------------------------------------------------------------------------------------------------------------
// Function to print vectors
template<typename T>
void print_vector(std::vector<T> v){
    std::cout<<"[ ";
    for(T &item : v){
        std::cout<<item<<" ";
    }
    std::cout<<"]"<<std::endl;
}
//---------------------------------------------------------------------------------------------------------------------
// (1) SparseMatrix definitions
// SparseMatrix default constructor
template <typename T>
SparseMatrix<T>::SparseMatrix(const unsigned int &nr,
                              const unsigned int &nc,
                              const std::vector<T> &d,
                              const std::vector<unsigned int> &c):n_rows(nr), n_cols(nc),values(d),cols(c) {};

// SparseMatrix copy constructor
template <typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> &other):
    n_rows(other.n_rows), n_cols(other.n_cols), values(other.values), cols(other.cols) {};

// SparseMatrix assignment operator
template <typename T>
SparseMatrix<T> & SparseMatrix<T>::operator =(const SparseMatrix<T> &other){
    if(this != &other){
        n_rows= other.n_rows;
        n_cols= other.n_cols;
        values= other.values;
        cols = other.cols;
        return (*this);
    }
    return (*this);
}
//---------------------------------------------------------------------------------------------------------------------
// (2) SparseMatrixCOO definitions
// SparseMatrixCOO default constructor
template <typename T>
SparseMatrixCOO<T>::SparseMatrixCOO(const unsigned int &nr,
                                    const unsigned int &nc,
                                    const std::vector<T> &d,
                                    const std::vector<unsigned int> &c,
                                    const std::vector<unsigned int> &r) : SparseMatrix<T>(nr, nc, d, c), rows(r) {};

// SparseMatrixCOO copy constructor
template <typename T>
SparseMatrixCOO<T>::SparseMatrixCOO(const SparseMatrixCOO<T> &other)
    :SparseMatrix<T>(other), rows(other.rows){};

// SparseMatrixCOO assignment operator
template <typename T>
SparseMatrixCOO<T> & SparseMatrixCOO<T>::operator =(const SparseMatrixCOO<T> &other){
    if(this != &other){
        this->n_rows= other.n_rows;
        this->n_cols= other.n_cols;
        this->values= other.values;
        this->rows= other.rows;
        this->cols= other.cols;
        return (*this);
    }
    return (*this);
}   
// Method to print information about a SparseMatrixCOO
template <typename T>
void SparseMatrixCOO<T>::get_info()const{
    std::cout<<std::endl;
    std::cout<<"Number of nonzero elements: "<< this->get_nzeros()<<std::endl;

    std::cout<<"Nonzero values: ";
    print_vector<T>(this->values); 

    std::cout<<"Columns: ";
    print_vector<unsigned int>(this->cols);

    std::cout<<"Rows: ";
    print_vector<unsigned int>(this->rows);  
}

// Method to print a SparseMatrixCOO
/* If either rows or columns are >10, we decided to print just the sparse values!*/  
template <typename T>                 
void SparseMatrixCOO<T>::print(){
    std::cout<<std::endl;
    // First we check matrix dimensions 
    // Case 1: both dimensions are <= 10 (we print the whole matrix)
    if(this->get_nrows()<=10 && this->get_ncols()<=10){
        // We can simply iterate over rows and columns and use the access operator for each (i,j) entry
        for (unsigned int i = 0; i < this->get_nrows(); i++) {
            std::cout<< "|  "; //this is just to have a "prettier" print
            for (unsigned int j = 0; j < this->get_ncols(); j++) {
                std::cout<<(*this)(i,j)<< "  ";
            }
            std::cout<< "|" <<std::endl;
        }
    }
    // Case 2: at least one dimension exceeds 10 (we print just the sparse values)
    else{
        std::cout<<"Matrix too large: only sparse values will be printed!"<<std::endl;
        // We just need to iterate over the values vector
        for (unsigned int i=0; i<this->get_nzeros(); i++){
            std::cout<<"["<< rows[i]<< ","<< this->cols[i]<<"] = "<< this->values[i]<<std::endl;
            
        }
        std::cout<<std::endl;
    }
}

// Operator for COO matrix-vector product
template <typename T>
const std::vector<T> SparseMatrixCOO<T>::operator*(const std::vector<T> &vec) const{
    // Check if matrix and vector dimensions are consistent 
    assert(vec.size()==this->n_cols);
    // Result vector of length=n_rows (initialized with zeros)
    std::vector<T> result(this->n_rows, 0);
    // This is a simplified version of the classic matrix-vector product which consider just nonzero values!
    for (unsigned int i=0; i<this->values.size(); i++){
        result[rows[i]] += this->values[i] * vec[this->cols[i]];
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
// (3) SparseMatrixCSR definitions
// SparseMatrixCSR class constructor
template <typename T>
SparseMatrixCSR<T>::SparseMatrixCSR(const unsigned int &nr,
                                    const unsigned int &nc,
                                    const std::vector<T> &d,
                                    const std::vector<unsigned int> &c,
                                    const std::vector<unsigned int> &r) : SparseMatrix<T>(nr, nc, d, c), rows_idx(r) {};

// SparseMatrixCSR copy constructor
template <typename T>
SparseMatrixCSR<T>::SparseMatrixCSR(const SparseMatrixCSR<T> &other)
    :SparseMatrix<T>(other), rows_idx(other.rows_idx) {};

// SparseMatrixCSR assignment operator
template <typename T>
SparseMatrixCSR<T> & SparseMatrixCSR<T>::operator =(const SparseMatrixCSR<T> &other){
    if(this != &other){
        this->n_rows= other.n_rows;
        this->n_cols= other.n_cols;
        this->values= other.values;
        this->cols= other.cols;
        this->rows_idx= other.rows_idx;
        return (*this);
    }
    return (*this);
}

// Method to print information about a SparseMatrixCSR
template <typename T>
void SparseMatrixCSR<T>::get_info() const{
    std::cout<<std::endl;
    std::cout<<"Number of nonzero elements: "<<this-> get_nzeros()<<std::endl;

    std::cout<<"Nonzero values: ";
    print_vector<T>(this->values); 

    std::cout<<"Columns: ";
    print_vector<unsigned int>(this->cols);

    std::cout<<"Rows_idx: ";
    print_vector<unsigned int>(this->rows_idx);  
}

// Method to print a SparseMatrixCSR  
template <typename T>                
void SparseMatrixCSR<T>::print(){
    std::cout<<std::endl;
    // First we check matrix dimensions 
    // Case 1: both dimensions are <= 10 (we print the whole matrix)
    if(this->n_rows<=10 && this->n_cols<=10){
        // We can simply iterate over rows and columns and use the access operator for each (i,j) entry
        for (unsigned int i = 0; i < this->n_rows; i++) {
            std::cout<< "|  ";
            for (unsigned int j = 0; j < this->n_cols; j++) {
                std::cout<<(*this)(i,j)<< "  ";
            }
            std::cout<< "|" <<std::endl;
        }
    }
    // Case 2: at least one dimension exceeds 10 (we print just the sparse values)
    else{
        std::cout<<"Matrix too large: only sparse values will be printed!"<<std::endl;
        // For each row of the matrix
        for(unsigned int i = 0; i < this->rows_idx.size() - 1; i++) {
            /* Just consider the "range" of columns associated to nonzero values (of that row!)
                    ~col_start will be rows_idx[i];
                    ~col_end will be rows_idx[i + 1];
            Iteration over "selected" columns */
            for (unsigned int j = this->rows_idx[i]; j < this->rows_idx[i+1]; j++) {
                // Print the value (i,j)
                std::cout << "[" << i << "," << this->cols[j] << "] = " << this->values[j] << std::endl;
            }
        }
        std::cout<<std::endl;
    }
    
}
// Operator for CSR matrix-vector product
template <typename T>
const std::vector<T> SparseMatrixCSR<T>::operator*(const std::vector<T> &vec)const {
    // Check if matrix and vector dimensions are consistent 
    assert(vec.size()==this->n_cols);
    // Result vector of length=n_rows (initialized with zeros)
    std::vector<T> result(this->n_rows, 0.0);    
    /*This is a simplified version of the classic matrix-vector product which consider just nonzero values!
      As seen in print function, to get nonzero values coordinates we just need to:
        - iterate over rows;
        - for each row, consider the column "range" [ rows_idx[i] , rows_idx[i+1] ] */
    for(unsigned int i=0; i<this->n_rows; i++){
        for (unsigned int j=this->rows_idx[i]; j<this->rows_idx[i+1]; j++){
            int col_idx= this->cols[j];
            result[i] += this->values[j] * vec[col_idx];
        }
    }
    return result;
}
//---------------------------------------------------------------------------------------------------------------------
// Functions for conversions
template <typename T>
SparseMatrixCSR<T>* COO_to_CSR(SparseMatrixCOO<T> *matrix){
    // Vectors attributes for the new matrix
    std::vector<unsigned int> new_cols;
    std::vector<T> new_values;
    std::vector<unsigned int> rows_idx;
    // CONVENTION: first element of rows_idx is always 0
    rows_idx.push_back(0);
    // Counter for the cumulative number of nonzero elements
    unsigned int counter=0;
    // Iteration over rows of the COO matrix
    for(unsigned int row=0; row < matrix->get_nrows(); row++) {
        // Iteration over nonzero values of the COO matrix
        for (unsigned int i = 0; i < matrix->get_nzeros(); i++) {
            // If the row of the current nonzero is equal to the current row (of the external loop)
            if (matrix->get_rows()[i] == row){
                new_cols.push_back(matrix->get_cols()[i]);
                new_values.push_back(matrix->get_values()[i]);
                counter++;
            }
        }
        rows_idx.push_back(counter);
    }

    // Now we can define the CSR version of the input matrix
    SparseMatrixCSR<T>* converted_matrix = new SparseMatrixCSR<T>{matrix->get_nrows(), matrix->get_ncols(), new_values, new_cols, rows_idx};
    /* Before returning the converted matrix(CSR), it makes sense to delete the initial COO version 
       Since we passed the input as a pointer, we can easily deallocate it with "delete" */
    delete matrix;
    return converted_matrix;
}

template <typename T>
SparseMatrixCOO<T>* CSR_to_COO(SparseMatrixCSR<T> *matrix){
    // Vectors attributes for the new matrix
    std::vector<unsigned int> cols=matrix->get_cols();
    std::vector<T> values=matrix->get_values();
    std::vector<unsigned int> rows;
    // For each row of the matrix
    for(unsigned i=0; i < matrix->get_nrows();i++){
        /* matrix->get_rows_idx()[i+1]-matrix->get_rows_idx()[i] is the number of non-zero elements in the i-th row,
           so we repeat its index in the rows vector such number of times */
        for(unsigned int rep=1;rep<=matrix->get_rows_idx()[i+1]-matrix->get_rows_idx()[i];rep++){
            rows.push_back(i);
        }
    }
    
    // Now we can define the COO version of the input matrix
    SparseMatrixCOO<T>* converted_matrix = new SparseMatrixCOO<T>{matrix->get_nrows(), matrix->get_ncols(), values, cols, rows};
    /* Before returning the converted matrix(COO), it makes sense to delete the initial CSR version 
       Since we passed the input as a pointer, we can easily deallocate it with "delete" */
    delete matrix;
    return converted_matrix;
}

#include "helper.hpp"