cmake_minimum_required(VERSION 3.5)

project(Cardan)

add_subdirectory(cardan)
add_subdirectory(test)

target_include_directories(cardan-lib PUBLIC .)
