cmake_minimum_required(VERSION 3.17)

project(CSP_solver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(CSP_solver main.cpp
                            csp.h csp.cpp
                            coloration.h coloration.cpp
                            arbre_dom.h arbre_dom.cpp
                            contrainte.h contrainte.cpp
                            build_in_problems.h build_in_problems.cpp
                            interface.h interface.cpp)
