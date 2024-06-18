#!/bin/bash

g++ -std=c++17 -Wall -Wpedantic main.cpp -o sparse_matrix

if [ $? -eq 0 ]; then
    echo "Build successful! You can run the program using ./sparse_matrix"
else
    echo "Build failed."
fi