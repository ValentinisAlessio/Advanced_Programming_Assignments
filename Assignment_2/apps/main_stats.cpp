//---------------------------------------------------------------------------------------------------------------------
// Libraries and modules
#include "dataframe.hpp"
#include <fstream> //to redirect the output
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
    //try-catch structure to handle exceptions!
    try{

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////// TAKE INPUT TERMINAL ARGUMENTS  ////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Check if the user inserted all the required inputs
        if(argc<3){ throw std::runtime_error("Not enough arguments provided from terminal. Retry!");}
        // Set variables related to input/output files 
        std::string outDir="../output/output_stats/";
        std::string dataDir="../datasets/";
        std::string csvFile=argv[1];
        std::string outFile=argv[2];

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////// CREATE DATAFRAME AND IMPORT CSV  //////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Dataframe data; //equivalent to Dataframe data{};
        data.import_csv(dataDir+csvFile);
        /* NB: equivalent to data.import_csv(filepath,','), since sep is set to ',' as default
            -Tried also to import data2.csv with ';' and worked
            -Tried also to import data3.csv with ' ' and worked
         Alternatively, single line initialization is allowed:
         Dataframe data{filepath,','},or just Dataframe data{filepath} */

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////// SETTINGS TO REDIRECT THE OUTPUT  //////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // If we arrived here the import phase has been successful! Print to terminal that dataset is being analyzed 
        std::cout<<"... analyzing "<<csvFile<<" ...."<<std::endl;
        // Open a file for writing
        std::ofstream outputFile(outDir+outFile);
        // Check if the output file is opened correctly
        if (!outputFile.is_open()) {
            std::cerr << "Whoops! Error in opening output file. Execution aborted!" << std::endl;
            return 1; // Return an error code
        }
        // From now we redirect the standard output to the output file
        std::streambuf *originalCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(outputFile.rdbuf());
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////// WRITING ON OUTPUT FILE  /////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
        std::cout<<  "         DATAFRAME OVERVIEW         "<<std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

        // Dataframe dimensions
        std::pair<unsigned int, unsigned int> dims = data.getDims();
        std::cout<<std::endl<<"Dataframe dimensions: "<<dims.first<<" rows, "<<dims.second<<" columns"<<std::endl;

        // Dataframe header
        std::vector<std::string> header= data.colnames();
        std::cout<<std::endl<<"Dataframe header: | ";
        for(const std::string &col : header){
            std::cout<<col<<" | ";
        }
        std::cout<<std::endl;

        // Dataframe head
        std::cout<<std::endl<<"Dataframe head (first 10 rows):"<<std::endl;
        data.head(10);

        // Dataframe tail
        std::cout<<std::endl<<"Dataframe tail (last 10 rows):"<<std::endl;
        data.tail(10);

        // Spots how many NaN for each attribute
        std::cout<<std::endl<<"NaN count for each attribute"<<std::endl;
        for(const auto & el : data.getData()){
            std::string col=el.first;
            std::cout<<"    - "<<col<<" has "<<data.countNaN(col)<< " NaN"<<std::endl;
        }
        std::cout<<std::endl<<std::endl<<std::endl;


        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
        std::cout<<  "        DATAFRAME STATISTICS        "<<std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

        // Dropping NaN values in order to avoid errors in computing correlations
        std::cout<<std::endl<<"... dropping NaN values to compute statistics ..."<<std::endl;
        data.dropNaN();
        // Show the user that NaN have been actually removed
        std::cout<<std::endl<<"NaN count for each attribute"<<std::endl;
        for(const auto & el : data.getData()){
            std::string col=el.first;
            std::cout<<"    - "<<col<<" has "<<data.countNaN(col)<< " NaN"<<std::endl;
        }
        std::cout<<std::endl<<std::endl;

        // Summary of all numerical attributes
        std::vector<std::string> numericalAttributes{"households", "housing_median_age", "latitude",
                                                     "longitude", "median_house_value","median_income",
                                                     "population", "total_bedrooms", "total_rooms"};
        for(const std::string &attribute : numericalAttributes){
            data.summary(attribute);
        }
        // Frequency table for the categorical variable
        data.table("ocean_proximity");
        
        // Covariance Matrix
        std::cout<<std::endl<<"Covariance matrix:"<<std::endl<<std::endl;
        data.covMat(numericalAttributes);

        // Correlation Matrix
        std::cout<<std::endl<<"Correlation matrix:"<<std::endl<<std::endl;
        data.corrMat(numericalAttributes);

        std::cout<<std::endl<<std::endl<<std::endl;


        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
        std::cout<<  "       OTHER FUNCTIONALITIES        "<<std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

        // Select rows by a condition and store them in another dataframe
        std::cout<<std::endl<<"(1) Filtering the rows where ocean_proximity is 'ISLAND' "<<std::endl<<std::endl;
        Dataframe data_island;
        data_island = data.filterRows("ocean_proximity","ISLAND");
        data_island.head(10); //NB: actually nrows=5, but head is able to detect it and print just 5 rows ;)
        
        // Add a column to data_island
        std::cout<<std::endl<<"(2) Adding a new column called Idx "<<std::endl<<std::endl;
        std::vector<ColumnValue> newcol{1.,2.,3.,4.,5.};
        // First call
        std::cout<<"First call"<<std::endl;
        data_island.addColumn("Idx",newcol);
        std::cout<<"Dataframe dimensions: "<<data_island.getDims().first<<" rows, "<<data_island.getDims().second<<" columns"<<std::endl;
        data_island.head(5);
        // Second call
        std::cout<<"Second call"<<std::endl;
        data_island.addColumn("Idx",newcol);
        std::cout<<std::endl<<"Dataframe dimensions: "<<data_island.getDims().first<<" rows, "<<data_island.getDims().second<<" columns"<<std::endl;
        data_island.head(5);

        // Drop the columns we inserted
        std::cout<<std::endl<<"(3) Dropping the two columns inserted "<<std::endl<<std::endl;
        data_island.dropColumn("Idx");
        data_island.dropColumn("Idx.copy");
        std::cout<<"Dataframe dimensions: "<<data_island.getDims().first<<" rows, "<<data_island.getDims().second<<" columns"<<std::endl;
        data_island.head(5);

        // Duplicate the last row of data_island
        std::cout<<std::endl<<"(4) Duplicating the last row of data_island "<<std::endl<<std::endl;
        unsigned int lastRowIdx= data_island.getDims().first;
        data_island.addRow(data_island.getRowByIdx(lastRowIdx),lastRowIdx+1);
        std::cout<<"Dataframe dimensions: "<<data_island.getDims().first<<" rows, "<<data_island.getDims().second<<" columns"<<std::endl;
        data_island.head(6);     

        // Example of multiple filtering
        std::cout<<std::endl<<"(5) Example of multiple filtering: rows in which ocean_proximity is 'ISLAND' and total_bedrooms=521.0"<<std::endl<<std::endl;
        Dataframe doubleFilteredDf= data.filterRows("ocean_proximity","ISLAND").filterRows("total_bedrooms",521.0);
        std::cout<<"Dataframe dimensions: "<<doubleFilteredDf.getDims().first<<" rows, "<<doubleFilteredDf.getDims().second<<" columns"<<std::endl;
        doubleFilteredDf.head(1);


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////// END OF WRITING  ///////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        // Restore the original standard output
        std::cout.rdbuf(originalCoutBuffer);
        // The file will be automatically closed when outputFile goes out of scope
        std::cout<<outFile<<" saved successfully in "<<outDir<<std::endl;
    }
    catch(const std::exception& e){
        std::cerr << "Whoops! Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
