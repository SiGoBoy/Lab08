cmake_minimum_required(VERSION 3.10...3.28
project(formatter_ex_lib)


include_directories(${CMAKE_SOURCE_DIR}/formatter_lib)

add_library(formatter_ex STATIC formatter_ex.cpp formatter_ex.h)
target_link_libraries(formatter_ex formatter)
