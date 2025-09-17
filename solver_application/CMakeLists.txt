cmake_minimum_required(VERSION 3.10...3.28)
project(solver_application)


include_directories(
    ${CMAKE_SOURCE_DIR}/formatter_lib
    ${CMAKE_SOURCE_DIR}/formatter_ex_lib
    ${CMAKE_SOURCE_DIR}/solver_lib
)

add_executable(solver_application equation.cpp)
target_link_libraries(solver_application solver formatter_ex formatter)

