#!/bin/bash

# Compile the program
clang++ pong.cpp -Ofast -w `sdl2-config --cflags --libs` -march=native -std=c++17 -Xpreprocessor -openmp -lomp -o pong
