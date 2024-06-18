if(STATS)
    #Create executable target
    add_executable(main_stats main_stats.cpp)
    #link my custom library
    target_link_libraries(main_stats stats_module)
endif()

if(MATH)
    #Create executable target
    add_executable(main_math main_math.cpp)
    #Include directories for Eigen
    target_include_directories(main_math PRIVATE ${EIG_DIR})
    #link my custom library
    target_link_libraries(main_math PRIVATE math_module Eigen3::Eigen)
endif()