cmake_minimum_required(VERSION 3.30)
project(planesComputation C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-O3 -march=native -funroll-loops -ffast-math)

add_executable(planesComputation main.c
        SETUP.h
        PLANES.h
        PLANES_ERROR.h
        PLANES_ERROR.c
        PLANES.c)
