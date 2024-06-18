// Include dataframe.hpp file
#include"dataframe.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////  HELPER METHODS AND FUNCTIONS  ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------------------------------
// 1) Helper method to convert a numerical column into a std::vector
std::vector<double> Dataframe::numColumnToStdVector(const std::string& attribute)const{
    std::vector<double> res;
    for (auto& val : this->dataset.at(attribute)) {
        if (val.has_value() && std::holds_alternative<double>(*val)) {
            res.push_back(std::get<double>(*val));
        }
    }
    return res;
}
//------------------------------------------------------------------------------------------------------------------------------
// 2) Helper method to spot if the input attribute is invalid
bool Dataframe::invalidAttributeName(const std::string& attribute) const{
    return (this->dataset.find(attribute) == this->dataset.end());
}
//------------------------------------------------------------------------------------------------------------------------------
// 3) Helper method to spot if the input row index is invalid
bool Dataframe::invalidRowIdx(const unsigned int& idx) const{
    //Note: unsigned int already guarantees that idx is not negative
    return ((idx==0) || (idx > this->nrows));    
}
//------------------------------------------------------------------------------------------------------------------------------
// 4) Helper method that spot if any of the attributes in a vector is invalid
bool Dataframe::anyInvalidAttribute(const std::vector<std::string>& attributes) const{
    //Check attributes validity: being a print function, I don't want to raise an exception
    bool anyInvalid = std::any_of(attributes.begin(), attributes.end(),
                                 [this](const std::string& attribute) {return this->invalidAttributeName(attribute);}
                                  );
    return anyInvalid;
}
//------------------------------------------------------------------------------------------------------------------------------
// 5) Helper free function to convert a ColumnValue into a std::string
std::string ColumnValueToString(const std::optional<std::variant<double,std::string>>& value){
   // i) If the value contains something
   if (value.has_value()) {
       // a) If numeric value-->convert it to string
       if (std::holds_alternative<double>(*value)) {
           return std::to_string(std::get<double>(*value));
       }
       // b) If string value--> just return itself
       return std::get<std::string>(*value);
   }
   // ii) If value is std::nullopt, as a convention, we convert it into the string "NA"
   return "NA";
}
//------------------------------------------------------------------------------------------------------------------------------
// 6) Helper free function to convert a std::vector<ColumnValue> into a std::vector<std::string>
std::vector<std::string> ColumnValueVectorToString(const std::vector<ColumnValue> &vector){
    //Initialize a result vector
    std::vector<std::string> res;
    // iterate over the input vector
    for(const auto &value : vector){
        // converted the current ColumnValue to string and append it to res
        res.push_back(ColumnValueToString(value));
    }
    return res;
}
//------------------------------------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////  DATAFRAME GENERAL METHODS  ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////  CONST METHODS  /////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------------------------------
// Method to get columns' names
std::vector<std::string> Dataframe::colnames() const{
    std::vector<std::string> res;
    // Iterate over map elements
    for (auto &map_el : this->dataset){
        // Append the current column name (so the current key) to res
        res.push_back(map_el.first);
    }
    return res;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to get dataframe's dimensions
std::pair<unsigned int,unsigned int> Dataframe::getDims()const{
    std::pair<unsigned int,unsigned int> dims=std::make_pair(this->nrows,this->ncols);
    return dims;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to get data "map"
std::map<std::string,std::vector<ColumnValue>> Dataframe::getData()const{
    return this->dataset;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to get values from a column
std::vector<ColumnValue> Dataframe::getColumn(const std::string& attribute)const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in getColumn(): input attribute does not belong to Dataframe.");
    }
    return this->dataset.at(attribute);
}
//------------------------------------------------------------------------------------------------------------------------------
//Method to get values from a row
std::vector<ColumnValue> Dataframe::getRowByIdx(const unsigned int &idx)const{
    // Check validity of the input index. If not valid, raise an error
    // Note: unsigned int already guarantees that idx is not negative
    if(this->invalidRowIdx(idx)){
        throw std::out_of_range("Error in getRowByIdx(): input idx incompatible w.r.t Dataframe size.");
    }
    // Getting the row
    std::vector<ColumnValue> row;
    for (auto& column : dataset) {
        row.push_back(column.second[idx-1]);
    }
    return row;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to counts the number of NAs/missing values of a given column
unsigned int Dataframe::countNaN(const std::string& attribute)const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in countNaN(): input attribute does not belong to Dataframe.");
    }
    // Declare a Nans counter
    unsigned int NaNs=0;
    // Iterate over elements of the column
    for (auto& val : this->dataset.at(attribute)) {
        // If the value is missing, increment the counter
        if (!val.has_value()){
            ++NaNs;
        }
    }
    return NaNs;
}
//------------------------------------------------------------------------------------------------------------------------------
void Dataframe::printRowByIdx(const unsigned int &idx) const{
    /* Check validity of the input index
    Note: this check is not mandatory, since getRowByIdx() already checks input correctness. However,
    getRowByIdx() would raise an exception, but here we just want to print the error and continue the execution
    */  
    if(this->invalidRowIdx(idx)){
        std::cout<<"Error in printRowByIdx(): input idx incompatible w.r.t Dataframe size."<<std::endl;
    }else{
        // Get the row
        std::vector<ColumnValue> row=getRowByIdx(idx);
        // Convert the row into a "string"-row
        std::vector<std::string> str_row= ColumnValueVectorToString(row);
        // Print its elements
        for (const auto& val : str_row) {
            std::cout << std::setw(20) << std::left << val << " ";
        }
        std::cout << std::endl;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
void Dataframe::printRow(const std::vector<ColumnValue>& row) const {
    // Convert the input row into a "string"-row
    std::vector<std::string> str_row= ColumnValueVectorToString(row);
    // Print its elements
    for (const auto& val : str_row) {
        std::cout << std::setw(20) << std::left << val << " ";
    }
    std::cout << std::endl;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to print values of a column
void Dataframe::printColumn(const std::string& attribute) const{
    /* Check validity of the input attribute
    Note: this check is not mandatory, since getColumn() already checks input correctness. However,
    getColumns() would raise an exception, but here we just want to print the error and continue the execution
    */
    if (this->invalidAttributeName(attribute)) {
        std::cout<<"Error in printColumn(): input attribute does not belong to Dataframe."<<std::endl;
    }else{
        // Get the column
        std::vector<ColumnValue> col=getColumn(attribute);
        // Convert it into a "string"-column
        std::vector<std::string> str_col= ColumnValueVectorToString(col);
        // Print its elements
        std::cout<<attribute<<": ";
        for (const auto& val : str_col) {
            std::cout << val << " ";
        }
        std::cout << std::endl;          
    }
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to print the last n rows of the dataframe
void Dataframe::head(const int& n) const {
    // If dataset is empty, just print that there are no rows to print!
    if (this->nrows==0) {
        std::cout<<"Empty dataset: no rows to print."<<std::endl;
    }
    // If dataset is not empty
    else{
        //Check input validity
        /*Note: here we don't check with invalidRowIdx since it would return false if n>nrows 
                and in such case we decided to print min(n,nrows)]*/
        if(n<0){ 
            std::cout<<"Error in function head(): input idx incompatible w.r.t Dataframe size."<<std::endl;
        }else{
            // 1) Print header 
            std::string sepline(this->ncols * 21, '-');
            std::cout<<sepline<<std::endl;
            for (const auto& col : this->colnames()) {
                std::cout << std::setw(20) << std::left << col << " "; 
            }
            std::cout<<std::endl<<sepline<<std::endl;
            // 2) Print values
            //Print until the minimum between n and this->nrows is reached
            for(unsigned int r=1;r <=std::min((unsigned int)n,this->nrows);++r){
                this->printRowByIdx(r);
            }
            /* Alternatively, we could use the RowIterator as:
                unsigned int count = 0;
                for(const auto& row : *this){ //Using RowIterator
                    printRow(row);
                    ++count;
                    if (count >= n) {
                        break;
                    }
                }
            */
            std::cout<<sepline<<std::endl<<std::endl;
        }
    } 
}
//------------------------------------------------------------------------------------------------------------------------------
void Dataframe::tail(const int& n) const {
    //If dataset is empty, just print that there are no rows to print!
    if (this->nrows==0) {
        std::cout<<"Empty dataset: no rows to print."<<std::endl;
    }
    // If dataset is not empty
    else{
        //Check input validity(we don't check with invalidRowIdx for the motivations explained in head() )
        if(n<0){ 
            std::cout<<"Error in function tail(): input idx incompatible w.r.t Dataframe size."<<std::endl;
        }else{
            // 1) Print header 
            std::string sepline(this->ncols * 21, '-');
            std::cout<<sepline<<std::endl;
            for (const auto& col : this->colnames()) {
                std::cout << std::setw(20) << std::left << col << " "; 
            }
            std::cout<<std::endl<<sepline<<std::endl;
            // 2) Print values
            //Print until the maximum between 1 and this->nrows-n is reached
            for(unsigned int r=this->nrows;r >std::max(static_cast<unsigned int>(1),this->nrows-(unsigned int)n);--r){
                this->printRowByIdx(r);
            }
            std::cout<<sepline<<std::endl<<std::endl;
        }
    } 
}
//------------------------------------------------------------------------------------------------------------------------------
//Method to filter rows by an attribute condition
Dataframe Dataframe::filterRows(const std::string &attribute,const ColumnValue &val) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in filterRows(): input attribute does not belong to Dataframe.");
    }
    // Get the header 
    std::vector<std::string> header=this->colnames();
    // Declare a result Dataframe 
    Dataframe result;
    // Since result is still empty, let's set its header
    result.setHeader(header);
    // Get the input attribute position (index) in the header vector
    size_t col_idx= std::find(header.begin(), header.end(), attribute) - header.begin();
    // Convert the input value to string (to allow making a coparison later)
    std::string str_val= ColumnValueToString(val);
    // Iterate over rows
    for(const auto& row : *this){
        // Convert the current row to a string row
        std::vector<std::string>str_row=ColumnValueVectorToString(row);
        // If the row respects the input condition, add it to result dataframe
        if(str_row[col_idx]==str_val){
            result.addRow(row,result.nrows +1);
        }
    } 
    return result;
}

/////////////////////////////////////////////////// NON-CONST METHODS  //////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------------------------------
//Method to import a csv file and store it as class attribute
void Dataframe::import_csv(const std::string &input_filepath,const char &sep){ //the sep default is already in the hpp file
    
    // BEGIN PARSING
    std::ifstream input_file(input_filepath);
    // If filepath doesn't match, raise an exception
    if (!input_file.is_open()) {
        throw std::runtime_error("Error in import_csv()! Check the correctness of the input csv.");
    }
    // If valid filepath:
    else{ 
        // (1) STORE COLUMN NAMES
        // Get the header
        std::string header;
        std::getline(input_file, header); // Get the first line for column names
        // Create a stringstream from the header
        std::istringstream iss(header);
        // Temporary string to store each column name
        std::string column_name;
        // Vector that will contain the column names of the csv
        std::vector<std::string> attributes{};
        // Use getline with ',' as the delimiter to split the header into words
        while (std::getline(iss, column_name,sep)) {
            //Increase ncols by 1
            ++this->ncols;
            // We can add the current column name as a key to the map
            this->dataset[column_name] = std::vector<ColumnValue>(); //empty vector at the moment
            // We can add the current column name also to the list of attributes
            attributes.push_back(column_name);
        }

        // (2) STORE COLUMN VALUES
        std::string line;
        // Iteration over rows
        while(std::getline(input_file,line)){
            // Increase nrows by 1
            ++this->nrows;
            std::istringstream iss(line);
            // Iterate over current line values and store the current word in the variable word
            std::string value;
            for(unsigned int attr=0;attr<attributes.size();++attr){ 
                // For loop instead of while in order to manage missing values in the last column of the csv!
                std::getline(iss,value,sep);
                // See if the value is convertible to string
                std::stringstream ss(value);
                double numericvalue;
                // Case 1: conversion to numeric is successful
                if (ss >> numericvalue) { 
                    if(std::isnan(numericvalue)){
                        // If value is NaN, add it as std::nullopt
                        ((this->dataset).at(attributes[attr])).push_back(std::nullopt);
                    }else{
                        ((this->dataset).at(attributes[attr])).push_back(numericvalue);
                    }
                }
                // Case 1: conversion to numeric fails-->so we just keep the string
                else {
                    // If string is empty or string is NA, add std::nullopt
                    // Note: nan, Nan and NaN were already managed in the previous if with std::isnan(numericvalue)
                    if((value=="") || (value=="NA")){
                        ((this->dataset).at(attributes[attr])).push_back(std::nullopt);
                    }
                    // If string is not empty, add it
                    else{
                        ((this->dataset).at(attributes[attr])).push_back(value);
                    }
                    
                }  
            }
        }
    }
    //Ok, file parsed! We can close the input file!
    input_file.close();
    std::cout<<"Data imported successfully!"<<std::endl;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to set a header to an empty Dataframe
void Dataframe::setHeader(const std::vector<std::string> &header){
    // If the Dataframe is empty
    if((this->dataset).empty()){
        for(const std::string &attr : header){
            this->dataset[attr]= std::vector<ColumnValue>();
            ++this->ncols;
        }
    }
    else{
        throw std::runtime_error("Error in setHeader(): dataframe has already an header!");
    }
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to add a row at a certain idx
// Note: the order in which map stores values must be respected!!!
void Dataframe::addRow(const std::vector<ColumnValue> &values, const unsigned int &idx){
    // Check validity of the input index
    // Note: here we cannot use invalidRowIdx(), since we may want to insert at idx=nrows+1)
    if(idx==0 || idx>this->nrows+1){
        throw std::out_of_range("Error in addRow(): invalid input index.");
    }
    // Check validity of the input row size
    if (values.size() != this->ncols){
        throw std::invalid_argument("Error: Invalid number of row elements.");
    }
    // Get the header of the dataframe
    std::vector<std::string> header=this->colnames();
    // For each column, add the value of the new ros
    for (size_t i = 0; i < values.size(); ++i) {
        ((this->dataset).at(header[i])).insert( (this->dataset[header[i]]).begin() + (idx-1), values[i]);
    }
    // Since we are adding a row, remember to update nrows attribute
    ++this->nrows;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to add a Column
void Dataframe::addColumn(const std::string &attribute, const std::vector<ColumnValue> &values){
    //Check on values size (must be = nrows!)
    if (values.size()!= this->nrows){
        throw std::invalid_argument("Error in addColumn(): values size incompatible w.r.t Dataframe.");
    }
    //If I try to add an attribute that already exist, I will add it as attribute.copy (instead of throwing an error)
    if (this->dataset.find(attribute) != this->dataset.end()){
        this->addColumn(attribute+".copy", values);
    }else{
        // Add the column to the map
        this->dataset[attribute]=values;
        // Since we are adding a column, remember to update ncols attribute
        ++this->ncols;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//Method to drop a row
void Dataframe::dropRowByIdx(const unsigned int &idx){
    // Check validity of the input idx. If not valid, raise an error
    if(this->invalidRowIdx(idx)){
        throw std::out_of_range("Error in dropRowByIdx(): invalid input index.");
    }
    // If valid input, iterate over columns
    for (auto& column : dataset) { 
        // and drop the value belonging to the input row
        (column.second).erase( (column.second).begin() + (idx-1));
    }
    // Since we are dropping a row, remember to update ncols attribute
    this->nrows--;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to drop a Column by attribute
void Dataframe::dropColumn(const std::string &attribute){
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in dropColumn(): input attribute does not belong to Dataframe.");
    }
    // If valid input, delete the the column from dataset
    this->dataset.erase(attribute);
    // Since we are dropping a column, remember to update ncols attribute
    --this->ncols;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to drop all the rows containing at least a NaN/missing value
void Dataframe::dropNaN(){
    // Iterate over rows
    for(unsigned int r= this->nrows;r>0;--r){ 
        // Flag variable to spot if the current row has at least a NaN (at first set to false)
        bool currRowHasNaN = false;
        // Get the current line
        std::vector<ColumnValue> row= this->getRowByIdx(r);
        // Iterate over current row elements
        for(auto &el : row){
            // If we spot a Nan, set the flag to true and break the loop
            if(!el.has_value()){
                currRowHasNaN=true;
                break;
            }
        }
        // If a NaN has been found, erase the current row
        if(currRowHasNaN){
            this->dropRowByIdx(r);
            // Note: dropRowByIdx() automatically decrease by 1 the attribute nrows
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////  DATAFRAME STATISTICS METHODS  /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the sum of a (numerical) column
double Dataframe::computeSum(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in function computeSum(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()){
        throw std::runtime_error("Error in function computeSum(): no numerical values found in the column.");
    }
    // If everything is ok, return the sum
    return std::accumulate(values.begin(),values.end(),0.0);
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the mean of a (numerical) column
double Dataframe::computeMean(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computeMean(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computeMean(): no numerical values found in the column.");
    }
    // If everything is ok, return the mean
    return gsl_stats_mean(values.data(), 1, values.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the min of a (numerical) column
double Dataframe::computeMin(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computeMin(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computeMin(): no numerical values found in the column.");
    }
    // If everything is ok, return the min
    return gsl_stats_min(values.data(), 1, values.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the max of a (numerical) column
double Dataframe::computeMax(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computeMax(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computeMax(): no numerical values found in the column.");
    }
    // If everything is ok, return the max
    return gsl_stats_max(values.data(), 1, values.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the median of a (numerical) column
double Dataframe::computeMedian(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computeMedian(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computeMedian(): no numerical values found in the column.");
    }
    // If everything is ok, return the median
    return gsl_stats_median(values.data(), 1, values.size());
}
//------------------------------------------------------------------------------------------------------------------------------
double Dataframe::computePercentile(const std::string& attribute, const double &p) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computePercentile(): input attribute does not belong to Dataframe.");
    }
    /* Check validity of the input percentile 
       Note: gsl Percentile function accepts also p<0 (it truncates it to 0) and p>100 (it truncates to 100)
       But we decided to made our function to raise an exception in case of invalid p!
    */
    if (p<0 || p>100) {
        throw std::invalid_argument("Error in computePercentile(): p must belong to [0,100].");
    }
    // Convert the column into a std::vector of doubles    
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computePercentile(): no numerical values found in the column.");
    }
    // If everything is ok, sort the vector (needed in order to apply gsl quantile function)
    std::sort(values.begin(),values.end());
    // Return the quantile
    return  gsl_stats_quantile_from_sorted_data(values.data(),1, values.size(), p/100);
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the variance of a (numerical) column
// Note: if column has just 1 value, the gsl library returns Nan without exceptions
double Dataframe::computeVariance(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computeVariance(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles  
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computeVariance(): no numerical values found in the column.");
    }
    // If everything is ok, return the variance
    return gsl_stats_variance(values.data(), 1, values.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the standard deviation of a (numerical) column
// Note: if column has just 1 value, the gsl library returns Nan without exceptions
double Dataframe::computeSd(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in computeSd(): input attribute does not belong to Dataframe.");
    }
    // Convert the column into a std::vector of doubles 
    std::vector<double> values= numColumnToStdVector(attribute);
    // If values does not contain any numerical value, raise an error
    if (values.empty()) {
        throw std::runtime_error("Error in function computeSd(): no numerical values found in the column.");
    }
    // If everything is ok, return the standard deviation
    return gsl_stats_sd(values.data(), 1, values.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute the covariance between two (numerical) columns
double Dataframe::computeCov(const std::string& attribute1,const std::string& attribute2)const{
    // Check validity of the input attributes. If not valid, raise an error
    if (this->invalidAttributeName(attribute1) || this->invalidAttributeName(attribute2)) {
        throw std::invalid_argument("Error in computeCov(): an input attribute does not belong to Dataframe.");
    }
    // Convert the columns into two std::vector of doubles 
    std::vector<double> values1= numColumnToStdVector(attribute1);
    std::vector<double> values2= numColumnToStdVector(attribute2);
    // If values does not contain any numerical value, raise an error
    if (values1.empty() || values2.empty()) {
        throw std::runtime_error("Error in computeCov(): no numerical values found in a column.");
    }
    // If sizes don't match, raise an error
    if (values1.size() != values2.size()) {
        throw std::runtime_error("Error in computeCov(): incompatible sizes to compute covariance.");
    }
    // If everything is ok, return the covariance
    return gsl_stats_covariance(values1.data(), 1, values2.data(),1,values1.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to compute pearson's correlation coefficient between two (numerical) columns
double Dataframe::computeCorr(const std::string& attribute1,const std::string& attribute2) const{
    // Check validity of the input attributes. If not valid, raise an error
    if (this->invalidAttributeName(attribute1) || this->invalidAttributeName(attribute2)) {
        throw std::invalid_argument("Error in computeCorr(): an input attribute does not belong to Dataframe.");
    }
    // Convert the columns into two std::vector of doubles 
    std::vector<double> values1= numColumnToStdVector(attribute1);
    std::vector<double> values2= numColumnToStdVector(attribute2);
    // If values does not contain any numerical value, raise an error
    if (values1.empty() || values2.empty()) {
        throw std::runtime_error("Error in computeCorr(): no numerical values found in a column.");
    }
    // If sizes don't match, raise an error
    if (values1.size() != values2.size()) {
        throw std::runtime_error("Error in computeCorr(): incompatible sizes: cannot compute Pearson's correlation coefficient!");
    }
    // If everything is ok, return the correlation
    return gsl_stats_correlation(values1.data(), 1, values2.data(),1,values1.size());
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to print a covariance matrix between numerical attributes
void Dataframe::covMat(const std::vector<std::string>& attributes) const{
    // Check validity of the input attributes. If not valid, raise an error
    if (this->anyInvalidAttribute(attributes)){
        throw std::invalid_argument("Error in covMat(): ensure all attributes belong to Dataframe.");
    }
    //3 cases:
    //(1) Size attributes=1-->covariance is 1
    if(attributes.size()==1){
        std::cout<<"Cov("<<attributes[0]<<","<<attributes[0]<<")="<<this->computeVariance(attributes[0])<<std::endl;
    }
    //(1) Size attributes=2-->so we just print the single covariance 
    else if (attributes.size()==2){
        std::cout<<"Cov("<<attributes[0]<<","<<attributes[1]<<")="
                 <<this->computeCov(attributes[0],attributes[1])<<std::endl;
    }
    //(3) Size attributes>2-->print the triangular corrmat
    else{ 
        std::string sepline((attributes.size()+1) * 20, '-');
        //Print horizontally the header
        std::cout<<sepline<<std::endl;
        //empty space for top-right corner of the matrix
        std::cout<<std::setw(20) << std::left <<" ";
        for (const auto& attribute : attributes) {
            std::cout << std::setw(20) << std::left << attribute;
        }
        std::cout<<std::endl<<sepline<<std::endl;
        // Print the corr values
        for (size_t i = 0; i < attributes.size(); ++i) {
            std::cout << std::setw(20) << std::left << attributes[i];
            //Since we need a triangular matrix, j will iterate just until i
            for (size_t j = 0; j <= i; ++j) {
                // Compute and print correlation
                double covariance = computeCov(attributes[i], attributes[j]);
                std::cout << std::setw(20) << std::left << std::fixed << std::setprecision(3) << covariance;
            }
            std::cout << std::endl;
        }
        std::cout<<sepline<<std::endl;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to print a correlation matrix between numerical attributes
void Dataframe::corrMat(const std::vector<std::string>& attributes) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->anyInvalidAttribute(attributes)){
        throw std::invalid_argument("Error in corrMat(): ensure all attributes belong to Dataframe.");
    }
    //3 cases:
    //(1) Size attributes=1-->correlation is 1
    if(attributes.size()==1){
        std::cout<<"Cor("<<attributes[0]<<","<<attributes[0]<<")=1"<<std::endl;
    }
    //(1) Size attributes=2-->so we just print the single correlation 
    else if (attributes.size()==2){
        std::cout<<"Cor("<<attributes[0]<<","<<attributes[1]<<")="
                 <<this->computeCorr(attributes[0],attributes[1])<<std::endl;
    }
    //(3) Size attributes>2-->print the triangular corrmat
    else{ 
        std::string sepline((attributes.size()+1) * 20, '-');
        //Print horizontally the header
        std::cout<<sepline<<std::endl;
        //empty space for top-right corner of the matrix
        std::cout<<std::setw(20) << std::left <<" ";
        for (const auto& attribute : attributes) {
            std::cout << std::setw(20) << std::left << attribute;
        }
        std::cout<<std::endl<<sepline<<std::endl;
        // Print the corr values
        for (size_t i = 0; i < attributes.size(); ++i) {
            std::cout << std::setw(20) << std::left << attributes[i];
            //Since we need a triangular matrix, j will iterate just until i
            for (size_t j = 0; j <= i; ++j) {
                // Compute and print correlation
                double correlation = computeCorr(attributes[i], attributes[j]);
                std::cout << std::setw(20) << std::left << std::fixed << std::setprecision(3) << correlation;
            }
            std::cout << std::endl;
        }
        std::cout<<sepline<<std::endl;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to get frequency table
void Dataframe::table(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in table(): input attribute does not belong to Dataframe.");
    }
    // Delcare a map in which key=category(or value), and value= freq of such category(or value)
    std::map<std::string,unsigned int> table;
    // Fill the map
    for (auto& val : (this->dataset).at(attribute)) {
            // Convert the value into a string
            std::string str_val= ColumnValueToString(val);
            //How to avoid considering std::nullopt? Remember that ColumnValueToString converts std::nullopt as "NA"
            if(str_val!="NA"){
                table[str_val] ++; // Update the frequency
            }        
         }
    // Print the frequency table
    std::cout<<std::endl;
    std::cout<<"Frequency table of "<<attribute<<":"<<std::endl;
    std::cout<<"------------------------------------"<<std::endl;
    for(auto &el : table){
        std::cout<<std::left<<std::setw(15)<<el.first<<"| "<<el.second<<" items"<<std::endl;
    }
    std::cout<<"------------------------------------"<<std::endl<<std::endl;
}
//------------------------------------------------------------------------------------------------------------------------------
// Method to print a summary of a variable
void Dataframe::summary(const std::string& attribute) const{
    // Check validity of the input attribute. If not valid, raise an error
    if (this->invalidAttributeName(attribute)) {
        throw std::invalid_argument("Error in summary(): input attribute does not belong to Dataframe.");
    }
    // If everything is ok, print the summary stats
    std::string sepline(15*8,'-');
    std::cout<<std::endl;
    std::cout<<"Summary of "<<attribute<<":"<<std::endl;
    std::cout<<sepline<<std::endl;
    std::cout << std::setw(15) << std::left << "Min" << " "
              << std::setw(15) << std::left << "1st Q" << " "
              << std::setw(15) << std::left << "Median" << " "
              << std::setw(15) << std::left << "3rd Q" << " "
              << std::setw(15) << std::left << "Max" << " "
              << std::setw(15) << std::left << "Mean" << " "
              << std::setw(15) << std::left << "Variance" << " "
              << std::setw(15) << std::left << "Std.Dev" << " "<<std::endl;
    std::cout << std::setw(15) << std::left << computeMin(attribute) << " "
              << std::setw(15) << std::left << computePercentile(attribute,25) << " "
              << std::setw(15) << std::left << computeMedian(attribute) << " "
              << std::setw(15) << std::left << computePercentile(attribute,75) << " "
              << std::setw(15) << std::left << computeMax(attribute) << " "
              << std::setw(15) << std::left << computeMean(attribute) << " "
              << std::setw(15) << std::left << computeVariance(attribute) << " "
              << std::setw(15) << std::left << computeSd(attribute) << " "<<std::endl;
    std::cout<<sepline<<std::endl<<std::endl;       
}
//------------------------------------------------------------------------------------------------------------------------------