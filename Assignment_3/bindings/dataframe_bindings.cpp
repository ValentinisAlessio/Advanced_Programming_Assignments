#include "dataframe.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>
using ColumnValue = std::optional< std::variant<double,std::string> >;
namespace py = pybind11;

PYBIND11_MODULE(dataframe, m) {  
    py::class_<Dataframe>(m, "Dataframe")
        .def(py::init<>(),
            R"(Constructor for the Dataframe class

            Parameters:
                None
            )")
        //---------------------------------------------------------------------------------------------------------------
        // General Dataframe methods
        .def("colnames", &Dataframe::colnames)
        .def("printRow", &Dataframe::printRow)
        .def("printColumn", &Dataframe::printColumn, py::arg("attribute"),
            R"(Print the column of the Dataframe

            Parameters:
                attribute (string): Name of the column to print
            )")
        .def("head", &Dataframe::head, py::arg("n"),
            R"(Print the first n rows of the Dataframe

            Parameters:
                n (int): Number of rows to print
            )")
        .def("tail", &Dataframe::tail, py::arg("n"),
            R"(Print the last n rows of the Dataframe

            Parameters:
                n (int): Number of rows to print
            )")
        .def("countNaN", &Dataframe::countNaN, py::arg("attribute"),
            R"(Count the number of NaN values in the column of the Dataframe

            Parameters:
                attribute (string): Name of the column to count NaN values

            Returns:
                int: Number of NaN values in the column
            )")
        .def("getDims", &Dataframe::getDims,
            R"(Get the dimensions of the Dataframe

            Parameters:
                None

            Returns:
                tuple: Dimensions of the Dataframe (number of rows, number of columns)
            )")
        .def("getData", &Dataframe::getData,
            R"(Get the data of the Dataframe

            Parameters:
                None

            Returns:
                dict : Data of the Dataframe
            )")
        .def("getColumn", &Dataframe::getColumn, py::arg("attribute"),
            R"(Get the column of the Dataframe

            Parameters:
                attribute (string): Name of the column to get

            Returns:
                list: Column of the Dataframe
            )")
        .def("getRowByIdx", &Dataframe::getRowByIdx, py::arg("idx"),
            R"(Get the row of the Dataframe by index

            Parameters:
                idx (int): Index of the row to get

            Returns:
                list of values: Row of the Dataframe
            )")
        .def("filterRows", &Dataframe::filterRows, py::arg("attribute"), py::arg("value"),
            R"(Filter the rows of the Dataframe by a value in a column

            Parameters:
                attribute (string): Name of the column to filter
                value (float or string): Value to filter
            
            Returns:
                obj Dataframe: Dataframe with filtered rows
            )")
        .def("import_csv", &Dataframe::import_csv, py::arg("input_filepath"), py::arg("sep") = ',',
            R"(Import a CSV file into the Dataframe

            Parameters:
                input_filepath (string): Path of the CSV file to import
                sep (char): Separator of the CSV file (default: ',')
            )")
        .def("setHeader", &Dataframe::setHeader, py::arg("header"),
            R"(Set the header of the Dataframe

            Parameters:
                header (list of strings): Header to set
            )")
        .def("addRow", &Dataframe::addRow, py::arg("row"), py::arg("row_idx"),
            R"(Add a row to the Dataframe

            Parameters:
                row (list of values): Row to add
                row_idx (int): Index of the row to add
            )")
        .def("addColumn", &Dataframe::addColumn, py::arg("attribute"), py::arg("column"),
            R"(Add a column to the Dataframe

            Parameters:
                attribute (string): Name of the column to add
                column (list): Column to add
            )")
        .def("dropColumn", &Dataframe::dropColumn, py::arg("attribute"),
            R"(Drop a column from the Dataframe

            Parameters:
                attribute (string): Name of the column to drop
            )")
        .def("dropRowByIdx", &Dataframe::dropRowByIdx, py::arg("idx"),
            R"(Drop a row from the Dataframe by index

            Parameters:
                idx (int): Index of the row to drop
            )")
        .def("dropNaN", &Dataframe::dropNaN,
            R"(Drop rows with NaN values from the Dataframe

            Parameters:
                None
            )")
        //---------------------------------------------------------------------------------------------------------------
        // Statistics methods
        .def("computeSum", &Dataframe::computeSum, py::arg("attribute"),
            R"(Compute the sum of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the sum

            Returns:
                float: Sum of the values in the column
            )")
        .def("computeMean", &Dataframe::computeMean, py::arg("attribute"),
            R"(Compute the mean of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the mean

            Returns:
                float: Mean of the values in the column
            )")
        .def("computeMin", &Dataframe::computeMin, py::arg("attribute"),
            R"(Compute the minimum of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the minimum

            Returns:
                float: Minimum of the values in the column
            )")
        .def("computeMax", &Dataframe::computeMax, py::arg("attribute"),
            R"(Compute the maximum of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the maximum

            Returns:
                float: Maximum of the values in the column
            )")
        .def("computeMedian", &Dataframe::computeMedian, py::arg("attribute"),
            R"(Compute the median of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the median

            Returns:
                float: Median of the values in the column
            )")
        .def("computeVariance", &Dataframe::computeVariance, py::arg("attribute"),
            R"(Compute the variance of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the variance

            Returns:
                float: Variance of the values in the column
            )")
        .def("computePercentile", &Dataframe::computePercentile, py::arg("attribute"), py::arg("percentile"),
            R"(Compute the percentile of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the percentile
                percentile (float): Percentile to compute

            Returns:
                float: Percentile of the values in the column
            )")
        .def("computeSd", &Dataframe::computeSd, py::arg("attribute"),
            R"(Compute the standard deviation of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to compute the standard deviation

            Returns:
                float: Standard deviation of the values in the column
            )")
        .def("computeCov", &Dataframe::computeCov, py::arg("attribute1"), py::arg("attribute2"),
            R"(Compute the covariance of the values in two columns of the Dataframe

            Parameters:
                attribute1 (string): Name of the first column to compute the covariance
                attribute2 (string): Name of the second column to compute the covariance

            Returns:
                float: Covariance of the values in the two columns
            )")
        .def("computeCorr", &Dataframe::computeCorr, py::arg("attribute1"), py::arg("attribute2"),
            R"(Compute the correlation of the values in two columns of the Dataframe

            Parameters:
                attribute1 (string): Name of the first column to compute the correlation
                attribute2 (string): Name of the second column to compute the correlation

            Returns:
                float: Correlation of the values in the two columns
            )")
        .def("printCovMat", &Dataframe::printCovMat, py::arg("attributes"),
            R"(Print the covariance matrix of the values in two columns of the Dataframe

            Parameters:
                attributes (list of strings): Names of the columns to compute the covariance matrix
            )")
        .def("printCorrMat", &Dataframe::printCorrMat, py::arg("attributes"),
            R"(Print the correlation matrix of the values in two columns of the Dataframe

            Parameters:
                attributes (list of strings): Names of the columns to compute the correlation matrix
            )")
        .def("table", &Dataframe::table, py::arg("attribute"),
            R"(Print a table of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to print the table
            )")
        .def("summary", &Dataframe::summary, py::arg("attribute"),
            R"(Print a summary of the values in a column of the Dataframe

            Parameters:
                attribute (string): Name of the column to print the summary
            )")
        //---------------------------------------------------------------------------------------------------------------
        // Binding for printing Dataframe
        .def("__repr__", [](const Dataframe &d) {
            auto dims = d.getDims();
            if (dims.first == 0 || dims.second == 0) {
                return std::string("<Empty Dataframe>");
            } else {
                return std::string("<Dataframe with ")+std::to_string(dims.first)+" rows and "+std::to_string(dims.second)+" columns>";
            }
        })
        //---------------------------------------------------------------------------------------------------------------
        // Bindings for accessing values of a Dataframe
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
        // (3) Python access of type df[attribute] (NB: it is the same as getColumn, but will be more user-friendly in Python)
        .def("__getitem__", [](const Dataframe &d, const std::string &attribute) {
            return d.getColumn(attribute);
        })
        // (4) Python access of type df[rowIdx] (NB: it is the same as getRowByIdx, but will be more user-friendly in Python)
        .def("__getitem__", [](const Dataframe &d, const unsigned int &idx) {
            return d.getRowByIdx(idx);
        })
        //---------------------------------------------------------------------------------------------------------------
        // Bindings for setting values of a Dataframe
        // (1) Binding for setting a value in a Dataframe ENTRY
        .def("__setitem__", [](Dataframe &d, const std::pair<unsigned int, std::string> &idx, const ColumnValue &value) {
            d.setDfEntry(idx, value); // setDfEntry was a new method added to Dataframe class for HW3
        })
        // (2) Binding for changing an entire COLUMN of a Dataframe
        .def("__setitem__", [](Dataframe &d, const std::string &attribute, const std::vector<ColumnValue> &column) {
            d.setDfColumn(attribute, column); // setDfColumn was a new method added to Dataframe class for HW3
        })
        // (3) Binding for changing an entire ROW of a Dataframe
        .def("__setitem__", [](Dataframe &d, const unsigned int &idx, const std::vector<ColumnValue> &row) {
            d.setDfRow(idx, row); // setDfRow was a new method added to Dataframe class for HW3
        })
        //---------------------------------------------------------------------------------------------------------------
        // Binding for the function preallocDims (needed for conversion from Pandas to our class)
        .def("preallocDims", &Dataframe::preallocDims, py::arg("nrows"), py::arg("ncols"),
            R"(Preallocate the dimensions of the Dataframe

            Parameters:
                nrows (int): Number of rows to preallocate
                ncols (int): Number of columns to preallocate
            )"
        )
        //---------------------------------------------------------------------------------------------------------------
        // Binding of a copy method
        .def("copy", [](const Dataframe& self) { return self;},
        R"(Copy method for a Dataframe object)"
        );
        //---------------------------------------------------------------------------------------------------------------    
}