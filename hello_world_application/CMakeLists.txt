cmake_minimum_required(VERSION 3.10...3.28)
project(hello_world_application)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world formatter_ex_lib)
