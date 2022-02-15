cmake_minimum_required(VERSION 3.5)

project(cardan-lib)
set(CMAKE_CXX_STANDARD 17)

add_library(cardan-lib ScriptExecutionContext.cpp ScriptExecutionContext.hpp)
