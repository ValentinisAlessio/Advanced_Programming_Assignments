//---------------------------------------------------------------------------------------------------------------------
// Libraries
#include<iostream>
#include<vector>
#include<cassert>
#include<iomanip>
#include "include/SparseMatrix.hpp"
//---------------------------------------------------------------------------------------------------------------------
int main(){
    //Set the precision to which i want to print doubles
    std::cout << std::setprecision(2) << std::fixed;
    std::cout<<std::endl;
    std::cout << "/////////////////////////////////////////////////////////"<<std::endl;
    std::cout << "/////////////////////  COO TESTS ////////////////////////"<<std::endl;
    std::cout << "/////////////////////////////////////////////////////////"<<std::endl;
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "           DECLARATION OF A COO SPARSE MATRIX            "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::vector<double> values{3.1, 4, 5, 7.4, 2, 6};
    std::vector<unsigned int> cols{2,4,2,4,1,3};
    std::vector<unsigned int> rows{0,0,1,1,3,3};
    SparseMatrixCOO<double> *M_COO= new SparseMatrixCOO<double>{4,5, values,cols,rows};
    /*We declared is dynamically since we'll convert it later*/
    std::cout<< std::endl<<"Let's consider the following double matrix (M_COO):"<<std::endl;
    M_COO->print();
    M_COO->get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "              READING FROM COO MATRIX TEST               "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<"M_COO(0,2): "<<(*M_COO)(0,2)<<std::endl;
    std::cout<<"M_COO(0,0): "<<(*M_COO)(0,0)<<std::endl;
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "                WRITING ON COO MATRIX TEST               "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;

    // (1) Overwrite a nonzero element with another nonzero
    std::cout<<"TEST(1): overwrite a nonzero element with another nonzero"<<std::endl<<std::endl;
    (*M_COO)(0,2)=3;
    std::cout<<"M_COO(0,2) now updated to: "<<(*M_COO)(0,2)<<std::endl;
    std::cout<<"Now matrix has changed to:"<<std::endl;
    M_COO->print();
    M_COO->get_info();
    std::cout<<std::endl;

    // (2) "Overwrite" a zero element with another zero
    std::cout<<"TEST(2): \"overwrite\" a zero element with another zero"<<std::endl<<std::endl;
    (*M_COO)(0,0)=0;
    std::cout<<"M_COO(0,0) now \"updated\" to: "<<(*M_COO)(0,0)<<std::endl;
    std::cout<<"Now matrix has \"changed\" to:"<<std::endl;
    M_COO->print();
    M_COO->get_info();
    std::cout<<std::endl;

    // (3) Overwrite a zero element with a nonzero
    std::cout<<"TEST(3): overwrite a zero element with a nonzero"<<std::endl<<std::endl;
    (*M_COO)(0,0)=4;
    std::cout<<"M_COO(0,0) now updated to: "<<(*M_COO)(0,0)<<std::endl;
    std::cout<<"Now matrix has changed to:"<<std::endl;
    M_COO->print();
    M_COO->get_info();
    std::cout<<std::endl;
    
    // (4) Overwrite a nonzero element with a zero
    std::cout<<"TEST(4): overwrite a nonzero element with a zero"<<std::endl<<std::endl;
    (*M_COO)(0,0)=0;
    std::cout<<"M_COO(0,0) now updated to: "<<(*M_COO)(0,0)<<std::endl;
    std::cout<<"Now matrix has changed to:"<<std::endl;
    M_COO->print();
    M_COO->get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "          MATRIX(COO)-VECTOR MULTIPLICATION TEST         "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    /*Test 1: multiplication with v=[1,...,1]*/
    std::vector<double> vec{1.,1.,1.,1.,1.};
    std::vector<double> res = (*M_COO)*vec;
    std::cout<<std::endl<<"Multiplication by [1,1,1,1,1]: ";
    print_vector<double>(res);
    /*Test 2: multiplication with e2*/
    std::vector<double> e2{0.,1.,0.,0.,0.};
    res = (*M_COO)*e2;
    std::cout<<std::endl<<"Multiplication by [0,1,0,0,0]: ";
    print_vector<double>(res);
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "        ALTERNATIVE PRINT FOR LARGE COO MATRICES         "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    SparseMatrixCOO<double> M2_COO{15,15,values,cols,rows};
    std::cout<<"Print of a 15x15 COO sparse matrix"<<std::endl;
    M2_COO.print();

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "            COO ASSIGNMENT OPERATOR (=) TEST             "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    M2_COO = *M_COO;
    M2_COO.print();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "                   COO TEMPLATE TEST                     "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::vector<int> int_val{3, 1, 5, 7, 2, 8};
    SparseMatrixCOO<int> INT_COO{4,5, int_val,cols, rows};
    std::cout<< std::endl<<"Let's consider the following int matrix (INT_COO):"<<std::endl;
    INT_COO.print();
    INT_COO.get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "/////////////////////////////////////////////////////////"<<std::endl;
    std::cout << "/////////////////////  CSR TESTS ////////////////////////"<<std::endl;
    std::cout << "/////////////////////////////////////////////////////////"<<std::endl;
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "           DECLARATION OF A CSR SPARSE MATRIX            "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::vector<unsigned int> rows_idx{0,2,4,4,6};
    SparseMatrixCSR<double> M_CSR{4,5,values,cols,rows_idx};
    std::cout<< "Let's consider the following double matrix (M_CSR):"<<std::endl;
    M_CSR.print();
    M_CSR.get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "              READING FROM CSR MATRIX TEST               "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<"M_CSR(0,2): "<<(M_CSR)(0,2)<<std::endl;
    std::cout<<"M_CSR(0,0): "<<(M_CSR)(0,0)<<std::endl;
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "                WRITING ON CSR MATRIX TEST               "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;

    // (1) Overwrite a nonzero element with another nonzero
    std::cout<<"TEST(1): overwrite a nonzero element with another nonzero"<<std::endl<<std::endl;
    (M_CSR)(0,2)=3;
    std::cout<<"M_CSR(0,2) now updated to: "<<(M_CSR)(0,2)<<std::endl;
    std::cout<<"Now matrix has changed to:"<<std::endl;
    M_CSR.print();
    M_CSR.get_info();
    std::cout<<std::endl;

    // (2) "Overwrite" a zero element with another zero
    std::cout<<"TEST(2): \"overwrite\" a zero element with another zero"<<std::endl<<std::endl;
    (M_CSR)(0,0)=0;
    std::cout<<"M_CSR(0,0) now \"updated\" to: "<<(M_CSR)(0,0)<<std::endl;
    std::cout<<"Now matrix has \"changed\" to:"<<std::endl;
    M_CSR.print();
    M_CSR.get_info();
    std::cout<<std::endl;

    // (3) Overwrite a zero element with a nonzero
    std::cout<<"TEST(3): overwrite a zero element with a nonzero"<<std::endl<<std::endl;
    (M_CSR)(0,0)=4;
    std::cout<<"M_CSR(0,0) now updated to: "<<(M_CSR)(0,0)<<std::endl;
    std::cout<<"Now matrix has changed to:"<<std::endl;
    M_CSR.print();
    M_CSR.get_info();
    std::cout<<std::endl;
    
    // (4) Overwrite a nonzero element with a zero
    std::cout<<"TEST(4): overwrite a nonzero element with a zero"<<std::endl<<std::endl;
    (M_CSR)(0,0)=0;
    std::cout<<"M_CSR(0,0) now updated to: "<<(M_CSR)(0,0)<<std::endl;
    std::cout<<"Now matrix has changed to:"<<std::endl;
    M_CSR.print();
    M_CSR.get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "          MATRIX(CSR)-VECTOR MULTIPLICATION TEST         "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    /*Test 1: multiplication with v=[1,...,1]*/
    res = (M_CSR)*vec;
    std::cout<<std::endl<<"Multiplication by [1,1,1,1,1]: ";
    print_vector<double>(res);
    /*Test 2: multiplication with e2*/
    res = (M_CSR)*e2;
    std::cout<<std::endl<<"Multiplication by [0,1,0,0,0]: ";
    print_vector<double>(res);
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "        ALTERNATIVE PRINT FOR LARGE CSR MATRICES         "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    SparseMatrixCSR<double> M2_CSR{15,15,values,cols,rows_idx};
    std::cout<<"Print of a 15x15 CSR sparse matrix"<<std::endl;
    M2_CSR.print();

    // ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "            CSR ASSIGNMENT OPERATOR (=) TEST             "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    M2_CSR = M_CSR;
    M2_CSR.print();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "                   CSR TEMPLATE TEST                     "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    SparseMatrixCOO<int> INT_CSR{4,5, int_val,cols, rows};
    std::cout<< std::endl<<"Let's consider the following int matrix (INT_CSR):"<<std::endl;
    INT_CSR.print();
    INT_CSR.get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "                COO to CSR CONVERSION TEST               "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    /*Tryout conversion*/
    SparseMatrixCSR<double>* M3_CSR = COO_to_CSR(M_COO);
    M3_CSR->print();
    M3_CSR->get_info();
    std::cout<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "---------------------------------------------------------"<<std::endl;
    std::cout<<  "                CSR to COO CONVERSION TEST               "<<std::endl;
    std::cout << "---------------------------------------------------------"<<std::endl;
    /*Tryout conversion*/
    SparseMatrixCOO<double>* M3_COO = CSR_to_COO(M3_CSR);
    M3_COO->print();
    M3_COO->get_info();
    std::cout<<std::endl;
    /*Since we have a dynamic allocation as a return from the convertion, we have to manually delete it
    NB: M_COO and M3_CSR had already been deallocated when converted!*/
    std::cout<<"Manual deallocation of the (remaining) matrices allocated dynamically:"<<std::endl;
    delete M3_COO;

    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << "/////////////////////////////////////////////////////////"<<std::endl;
    std::cout << "End of main(): destruction of the remaining matrices:"<<std::endl<<std::endl;

    ////////////////////////////////////////////////////////////////////////////////////  
    return 0;
}