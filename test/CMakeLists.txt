cmake_minimum_required(VERSION 3.5)

project(cardan-test)

add_subdirectory(${CMAKE_SOURCE_DIR}/googletest googletest)

add_executable(cardan-test ScriptExecutionContextTest.cpp)

target_include_directories(cardan-test PRIVATE googletest/googletest/include)
target_link_libraries(cardan-test PRIVATE gtest gtest_main)
