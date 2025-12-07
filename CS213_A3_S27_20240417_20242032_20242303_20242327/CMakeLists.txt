cmake_minimum_required(VERSION 3.31)
project(Assignment-3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR .)
include_directories(${SRC_DIR})

add_executable(Assignment_3
        ${SRC_DIR}/XO_Classes.cpp
        ${SRC_DIR}/XO_Demo.cpp
        ${SRC_DIR}/Numerical_Tic_Tac_Toe.cpp
        ${SRC_DIR}/SUS.cpp
        ${SRC_DIR}/FiveXO.cpp
        ${SRC_DIR}/FourXO.cpp
        ${SRC_DIR}/Obstacles.cpp
        ${SRC_DIR}/Misere_Tic_Tac_Teo.cpp
        ${SRC_DIR}/Four_in_a_row.cpp
        ${SRC_DIR}/Pyramid_Board.cpp
        ${SRC_DIR}/Infinity_Board.cpp
        ${SRC_DIR}/word.cpp
        ${SRC_DIR}/Ultimate_TicTacToe.cpp
        ${SRC_DIR}/Memory_TicTacToe.cpp
        ${SRC_DIR}/Diamond.cpp


)

file(COPY dic.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# Doxygen documentation target
# This block finds Doxygen and creates a convenient `doxygen` target that will
# run the Doxygen executable using the Doxyfile at the project root.
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_target(doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found - documentation target 'doxygen' will be unavailable")
endif()
