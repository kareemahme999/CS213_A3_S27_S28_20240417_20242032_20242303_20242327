cmake_minimum_required(VERSION 3.31.6)
project(BoardGameFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BoardGameFramework
        BoardGame_Classes.h
        XO_Classes.cpp
        XO_Classes.h
        XO_Demo.cpp
        Numerical_Tic_Tac_Toe.cpp)
