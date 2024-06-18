#Main project autocompilation file
cmake_minimum_required(VERSION 3.12)
project(custom_modules)

#Compile with C++17
set(CMAKE_CXX_STANDARD 17)
#Add compilation flags
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Find GSL and Eigen
find_package(GSL 2.7.1 REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED NO_MODULE)

#Include directories for GSL and Eigen
include_directories(${GSL_INCLUDE_DIRS})
include_directories(${EIGEN_INCLUDE_DIRS})

#Include Header directories
include_directories(include)

#Add subdirectories
add_subdirectory(source)
add_subdirectory(apps)

# Copy datasets and output directories to build directory
file(COPY ${CMAKE_SOURCE_DIR}/datasets DESTINATION ${CMAKE_BINARY_DIR})

# Delete output directory in the root if exists
#if(EXISTS ${CMAKE_SOURCE_DIR}/output)
#    message("Deleting existing output directory in the root...")
#    file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/output)
#endif()
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/output)
        # Create output directory into build
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

        #Create subdirectories within build/output/
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/output/output_stats)
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/output/output_math)
endif()

if(DEFINED STATS AND NOT STATS)
        set(NO_STATS_TARGET OFF)
endif()

if(DEFINED MATH AND NOT MATH)
        set(NO_MATH_TARGET OFF)
endif()

#Define an 'output_update' custom target to update the root output directory
add_custom_target(output_update ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/output ${CMAKE_BINARY_DIR}/output
        COMMENT "Updating build/output directory"
)

#Make the output_update target depend on the subdirectory targets

if(NOT DEFINED NO_STATS_TARGET)
        add_dependencies(output_update main_stats)
endif()

if(NOT DEFINED NO_MATH_TARGET)
        add_dependencies(output_update main_math)
endif()
