# Add options for specifying the directory of the library
option(GSL_DIR "Path to GSL library" "")
option(EIG_DIR "Path to Eigen library" "")

# Set the different source files for compilation
set(STAT_SRC dataframe.cpp)
set(MATH_SRC ExplicitODESolver.cpp Functions.cpp)

#Set two cache variables to allow independent compilation of the modules
option(STATS "Compile stats library" ON)
option(MATH "Compile math library" ON)

#Conditionally compile the modules
if(STATS)
    #Compile the stats module
    add_library(stats_module SHARED ${STAT_SRC})
    # Include directories for external libraries (GSL)
    target_include_directories(stats_module PRIVATE ${GSL_DIR})
    # Link GSL library to the module
    target_link_libraries(stats_module PRIVATE GSL::gsl GSL::gslcblas)
endif()

if(MATH)
    #Compile the math module
    add_library(math_module SHARED ${MATH_SRC})
    # Include directories for external libraries (Eigen)
    target_include_directories(math_module PRIVATE ${EIG_DIR})
    # Link Eigen library to the module
    target_link_libraries(math_module PRIVATE Eigen3::Eigen)
endif()



