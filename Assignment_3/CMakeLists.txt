cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(Homework3)

option(EIGEN3_DIR "Eigen3 include directory" "")
option(GSL_DIR "GSL include directory" "")
option(PYBIND11_DIR "Pybind11 include directory" "")

# Find pybind11
find_package(pybind11 REQUIRED)
if(PYBIND11_DIR)
    # Possibility to manually set directories
    message(STATUS "Using ${PYBIND11_DIR} to look for Pybind11 library")
    include_directories(${PYBIND11_DIR})
else()
    include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})
endif()

# Find Eigen3
if(EIGEN3_DIR)
    # Possibility to manually set directories
    message(STATUS "Using ${EIGEN3_DIR} to look for Eigen3 library")
    find_package(Eigen3 3.4.0 REQUIRED HINTS ${EIGEN3_DIR} NO_MODULE)
    set(EIGEN3_INCLUDE_DIR ${EIGEN3_DIR})
else()
    find_package(Eigen3 3.4.0 REQUIRED NO_MODULE)
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()
    
# Find GSL
find_package(GSL 2.7.1 REQUIRED)
if(GSL_DIR)
    # Possibility to manually set directories
    message(STATUS "Using ${GSL_DIR} to look for GSL library")
    #find_package(GSL 2.7.1 REQUIRED HINTS ${GSL_DIR})
    set(GSL_INCLUDE_DIR ${GSL_DIR})
else()
    include_directories(${GSL_INCLUDE_DIR})
endif()

message(STATUS "EIGEN3_DIR: ${EIGEN3_INCLUDE_DIR}")
message(STATUS "GSL_DIR: ${GSL_INCLUDE_DIR}")

# bind ODE module
pybind11_add_module(ODE source/ExplicitODESolver.cpp bindings/ODE_bindings.cpp)
target_include_directories(ODE PRIVATE include ${EIGEN3_INCLUDE_DIR})
# target_link_libraries(ODE PRIVATE Eigen3::Eigen) #NOT needed since Eigen is header only

# bind dataframe module
pybind11_add_module(dataframe source/dataframe.cpp bindings/dataframe_bindings.cpp)
target_include_directories(dataframe PRIVATE include ${GSL_INCLUDE_DIR})
target_link_libraries(dataframe PRIVATE GSL::gsl GSL::gslcblas)
