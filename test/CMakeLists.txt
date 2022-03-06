cmake_minimum_required(VERSION 3.5)

project(cardan-test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-DV8_COMPRESS_POINTERS")

add_subdirectory(${CMAKE_SOURCE_DIR}/googletest googletest)

add_executable(cardan-test
    ContextTest.cpp
    CardanTest.cpp
    ValueTest.cpp
    ArrayTest.cpp
    ObjectTest.cpp
    FunctionTest.cpp
    Scenarios.cpp
)

target_include_directories(cardan-test PRIVATE ../src googletest/googletest/include)
target_link_libraries(cardan-test PRIVATE cardan-lib gtest gmock pthread)
