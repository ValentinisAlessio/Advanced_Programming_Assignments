//---------------------------------------------------------------------------------------------------------------------
/* Some helper functions to make other class methods easier both to 
   implement and understand (both for COO and CSR format)
*/
//---------------------------------------------------------------------------------------------------------------------
// (I) COO Helper function to find the index of an element at position (i, j)
    /*We adopt the following convention:
        - if the element is non zero-->return its index in the values vector
        - if the element is zero (so it does not belong to the values vector)-->return -1
    */
template <typename T>
const int SparseMatrixCOO<T>::findIndex(const unsigned int i, const unsigned int j) const {
        for (unsigned int k = 0; k < this->values.size(); k++) {
            if (this->rows[k] == i && this->cols[k] == j) {
                return k;
            }
        }
        return -1; // Element not found
    }
//---------------------------------------------------------------------------------------------------------------------
// (II) COO Helper function to get the value at position (i, j)
template <typename T>
const T SparseMatrixCOO<T>::getValue(const unsigned int i, const unsigned int j) const {
        int index = findIndex(i, j);
        // Remember findIndex convention
        if (index != -1) { //if the value is nonzero
            return this->values[index];
        }
        return 0; // if index = -1 the element is 0
    }
//---------------------------------------------------------------------------------------------------------------------
// (III) COO Helper function to set the new value at position (i, j)
template <typename T>
void SparseMatrixCOO<T>::setValue(const unsigned int i, const unsigned int j, const T value) {
    int index = findIndex(i, j);
    // Basing on the value that we insert, there are 2 cases
    // Case 1: the value that we want to insert is 0
    if (value == 0) {
        if (index != -1) {
            // If the value is 0 and the element to replace is nonzero, remove it from the values vector
            this->values.erase(this->values.begin() + index);
            this->rows.erase(this->rows.begin() + index);
            this->cols.erase(this->cols.begin() + index);
        }
        // No else because if the value is 0 and the element to replace is 0, I don't have to do anything
    }
    // Case 2: the value that we want to insert is non zero
    else {
        if (index != -1) {
            // If the value is non-zero and the element to replace is nonzero, just update its value
            this->values[index] = value;
        }
        else{
            // If the value is non-zero and the element to replace was 0, we need to add it to the values vector
            this->values.push_back(value);
            this->rows.push_back(i);
            this->cols.push_back(j);
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
// (I) CSR Helper function to find the index of an element at position (i, j)
template <typename T>
const int SparseMatrixCSR<T>::findIndex(const unsigned int i, const unsigned int j) const {
    for(unsigned int k=this->rows_idx[i]; k<this->rows_idx[i+1]; k++) {
        if(this->cols[k] == j) {
            return k;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------------------------------------------------
// (II) CSR Helper function to get the value at position (i, j)
template <typename T>
const T SparseMatrixCSR<T>::getValue(const unsigned int i, const unsigned int j) const {
    int index = findIndex(i, j);
    if(index!= -1) {
        return this->values[index];
    }
    return 0.;
}
//---------------------------------------------------------------------------------------------------------------------
// (III) CSR Helper function to set the new value at position (i, j)
template <typename T>
void SparseMatrixCSR<T>::setValue(const unsigned int i, const unsigned int j, const T value) {
    int index = findIndex(i, j);
    // Basing on the value that we insert, there are 2 cases:
    // Case 1: the value that we want to insert is 0
    if(value == 0) {
        if(index!= -1) {
            // If the value is 0 and the element to replace is nonzero, remove it from the values vector
            this->values.erase(this->values.begin() + index);
            this->cols.erase(this->cols.begin() + index);
            // Update cumulative values in rows_idx vector
            for (unsigned int k = i+1; k < this->rows_idx.size(); k++){
                this->rows_idx[k]--;
            }
        }
        // No else because if the value is 0 and the element to replace is 0, we don't have to do anything
    }
    // Case 2: the value that we want to insert is non zero
    else {
        if(index!= -1) {
            // If the value is non-zero and the element to replace is nonzero, just update its value
            this->values[index] = value;
        }
        else {
            // If the value is non-zero and the element to replace was 0, we need to add it to the values vector
            unsigned int position = this->rows_idx[i];
            while(this->cols[position] < j) {
                position++;
            }
            this->values.insert(this->values.begin() + position, value);
            this->cols.insert(this->cols.begin() + position, j);
            for (unsigned int k = i+1; k < this->rows_idx.size(); k++){
                this->rows_idx[k]++;
            }
        }
    }
}