cmake_minimum_required(VERSION 3.5)

project(cardan-test)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/googletest googletest)

add_executable(cardan-test ScriptExecutionContextTest.cpp)

target_include_directories(cardan-test PRIVATE ../src googletest/googletest/include)
target_link_libraries(cardan-test PRIVATE cardan-lib gtest gtest_main)
