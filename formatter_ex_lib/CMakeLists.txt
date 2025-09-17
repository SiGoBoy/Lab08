cmake_minimum_required(VERSION 3.10...3.28)
project(formatter_ex_lib)

include_directories(${CMAKE_SOURCE_DIR}/formatter_lib)

add_library(formatter_ex_lib STATIC formatter_ex.cpp formatter_ex.h)

target_link_libraries(formatter_ex_lib formatter_lib)

target_include_directories(formatter_ex_lib PRIVATE ${CMAKE_SOURCE_DIR}/formatter_lib)
