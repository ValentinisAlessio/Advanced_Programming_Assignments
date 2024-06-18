#ifndef DATAFRAME_HPP_
#define DATAFRAME_HPP_
//--------------------------------------------------------------------------------
//Libraries
//--------------------------------------------------------------------------------
#include<iostream>
#include<fstream>
#include <sstream>
#include<map>
#include<algorithm>
#include<vector>
#include<string>
#include<optional>
#include<variant>
#include<numeric>
#include<cmath>
#include<iomanip>
#include<gsl/gsl_statistics.h>
using ColumnValue = std::optional< std::variant<double,std::string> >;
//--------------------------------------------------------------------------------

// Dataframe class
class Dataframe{
    public:
        //--------------------------------------------------------------------------------------------
        // Constructors and Destructor
        //--------------------------------------------------------------------------------------------

        // Constructor of an "empty" Dataframe
        Dataframe(){
            this->ncols=0;
            this->nrows=0;
            // header can be set later with the method setHeader()
        } 
        // Constructor by passing input filepath and (eventually) a sep
        Dataframe(const std::string &input_filepath,const char &sep=','){
            this->import_csv(input_filepath,sep);
        }
        // Copy constructor
        Dataframe(const Dataframe& other){
            if(this != &other){
                this->dataset=other.dataset;
                this->nrows=other.nrows;
                this->ncols=other.ncols;
            }
        }
        //Assignment operator
        Dataframe &operator =(const Dataframe &other){
            if(this != &other){
                this->dataset=other.dataset;
                this->nrows=other.nrows;
                this->ncols=other.ncols;
                return (*this);
            }
            return (*this);
        }
        //Default destructor
        ~Dataframe() = default;
        //--------------------------------------------------------------------------------------------


        //--------------------------------------------------------------------------------------------
        // RowIterator class
        //--------------------------------------------------------------------------------------------
        class RowIterator {
        public:
            RowIterator(const std::map<std::string, std::vector<ColumnValue>>& data, unsigned int idx)
                : dataset(data), currentidx(idx) {}
            // Operator to move to the next element
            RowIterator& operator++() {
                ++currentidx;
                return *this;
            }
            // Operator to dereference and get the current value
            std::vector<ColumnValue> operator*() {
                std::vector<ColumnValue> row;
                for (const auto& column : dataset) {
                    row.push_back(column.second[currentidx]);
                }
                return row;
            }
            // Operator to check if two RowIterators are equal
            bool operator!=(const RowIterator& other) const {
                return currentidx != other.currentidx;
            }
        private:
            const std::map<std::string, std::vector<ColumnValue>>& dataset;
            unsigned int currentidx;
        };
        // Method to get the begin RowIterator
        RowIterator begin() const {
            return RowIterator(dataset, 0);
        }
        // Method to get the end RowIterator
        RowIterator end() const {
            /*To ensure that the iteration covers all rows consistently, we should use the size of 
                the largest column vector as the reference for the end RowIterator*/
            unsigned int maxSize = 0; 
            for (const auto& column : dataset) {
                if (!column.second.empty() && column.second.size() > maxSize) {
                    maxSize = column.second.size();
                }
            }
            return RowIterator(dataset, maxSize);
        }
        //--------------------------------------------------------------------------------------------

        //--------------------------------------------------------------------------------------------
        // Dataframe general methods
        //--------------------------------------------------------------------------------------------
        
        // CONST methods

        // Method to get columns' names
        std::vector<std::string>  colnames() const;
        // Method to get dataframe's dimensions
        std::pair<unsigned int,unsigned int> getDims()const;
        // Method to get the data od the dataframe
        std::map<std::string,std::vector<ColumnValue>> getData()const;
        // Method to get values from a column
        std::vector<ColumnValue> getColumn(const std::string& attribute)const;
        //Method to get values from a row
        std::vector<ColumnValue> getRowByIdx(const unsigned int &idx)const;
        // Method to print a row given its index
        void printRowByIdx(const unsigned int &idx) const;
        // Method to print a row (useful in rows iteration)
        void printRow(const std::vector<ColumnValue>& row) const;
        // Method to print values of a column
        void printColumn(const std::string& attribute) const;
        // Method to print to terminal the first n rows of the dataframe
        void head(const int &n) const;
        // Method to print to terminal the last n rows of the dataframe
        void tail(const int &n) const;
        // Method to count the number of NaNs/missing values of a given column
        unsigned int countNaN(const std::string& attribute)const;
        //Method to filter rows by an attribute condition
        Dataframe filterRows(const std::string &attribute,const ColumnValue &val) const;

        // NON-CONST methods

        //Method to import a csv file
        void import_csv(const std::string &input_filepath,const char &sep=',');
        //Method to set a header to an empty Dataframe
        void setHeader(const std::vector<std::string> &header);
        // Method to add a row at a certain index
        void addRow(const std::vector<ColumnValue> &values, const unsigned int &idx);
        // Method to add a column
        void addColumn(const std::string &attribute, const std::vector<ColumnValue> &values);
        // Method to drop a row
        void dropRowByIdx(const unsigned int &idx);
        // Method to drop a column
        void dropColumn(const std::string &attribute);
        // Method to drop all rows containing NaNs or missing values
        void dropNaN();
        //--------------------------------------------------------------------------------------------


        //--------------------------------------------------------------------------------------------
        // Dataframe statistics methods
        //--------------------------------------------------------------------------------------------
        // Method to compute the sum of a (numerical) column
        double computeSum(const std::string& attribute) const;
        // Method to compute the mean of a (numerical) column
        double computeMean(const std::string& attribute) const;
        // Method to compute the min of a (numerical) column
        double computeMin(const std::string& attribute) const;
        // Method to compute the max of a (numerical) column
        double computeMax(const std::string& attribute) const;
        double computeMedian(const std::string& attribute) const;
        double computePercentile(const std::string& attribute, const double &p) const;
        // Method to compute the variance of a (numerical) column
        // NB: if column has just 1 value, the gsl library returns Nan without exceptions
        double computeVariance(const std::string& attribute) const;
        // Method to compute the standard deviation of a (numerical) column
        // NB: if column has just 1 value, the gsl library returns Nan without exceptions
        double computeSd(const std::string& attribute) const;
        // Method to compute the covariance between two (numerical) columns
        double computeCov(const std::string& attribute1,const std::string& attribute2)const;
        // Method to compute pearson's correlation coefficient between two (numerical) columns
        double computeCorr(const std::string& attribute1,const std::string& attribute2) const;
        // Method to print the covariance Matrix
        void covMat(const std::vector<std::string>& attributes) const;
        // Method to print the correlation Matrix
        void corrMat(const std::vector<std::string>& attributes) const;
        // Method to get a frequency table
        void table(const std::string& attribute) const;
        // Method to print a summary of a numeric variable
        void summary(const std::string& attribute) const;
        //--------------------------------------------------------------------------------------------

    private:
        //--------------------------------------------------------------------------------------------
        // Helper methods
        //--------------------------------------------------------------------------------------------
        // 1)Helper method to convert a Column into a std::vector
        std::vector<double> numColumnToStdVector(const std::string& attribute) const;
        // 2) Helper method that spots invalid attribute in inputs
        bool invalidAttributeName(const std::string& attribute) const;
        // 3) Helper method that spots invalid row index in inputs
        bool invalidRowIdx(const unsigned int& idx) const;
        // 4) Helper method that spots if a vector of attribute names contains any invalid attribute
        bool anyInvalidAttribute(const std::vector<std::string>& attributes) const;
        //--------------------------------------------------------------------------------------------

        //--------------------------------------------------------------------------------------------
        // Class attributes
        //--------------------------------------------------------------------------------------------
        std::map< std::string, std::vector<ColumnValue> > dataset;
        unsigned int nrows;
        unsigned int ncols;
        //--------------------------------------------------------------------------------------------
};
#endif