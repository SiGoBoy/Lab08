cmake_minimum_required(VERSION 3.10...3.28)
project(solver_lib)

add_library(solver_lib STATIC solver.cpp solver.h)
target_link_libraries(solver_lib formatter_lib)
