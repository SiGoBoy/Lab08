cmake_minimum_required(VERSION 3.10...3.28)
project(solver_application)

add_executable(solver_app equation.cpp)
target_link_libraries(solver_app solver_lib formatter_ex_lib)
