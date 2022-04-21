cmake_minimum_required(VERSION 3.5)

project(cardan-test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-DV8_COMPRESS_POINTERS")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../googletest googletest)

add_executable(cardan-test
    ContextTest.cpp
    ClassTest.cpp
    CardanTest.cpp
    ValueTest.cpp
    ArrayTest.cpp
    ObjectTest.cpp
    FunctionTest.cpp
    ConvertersTest.cpp
    Scenarios.cpp

    Scenarios/Converters/UserDefinedConverters.cpp
    Scenarios/Converters/UserDefinedConverters.hpp

    Scenarios/Converters/ValuesConversions.cpp
)

target_include_directories(cardan-test PRIVATE ../src googletest/googletest/include)
target_link_libraries(cardan-test PRIVATE cardan-lib gtest gmock pthread)
