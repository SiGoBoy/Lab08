cmake_minimum_required(VERSION 3.10...3.28)
project(hello_world_application)

# Добавьте пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/formatter_lib
    ${CMAKE_SOURCE_DIR}/formatter_ex_lib
)

add_executable(hello_world_application hello_world.cpp)
target_link_libraries(hello_world_application formatter_ex formatter)
