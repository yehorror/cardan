cmake_minimum_required(VERSION 3.5)

project(cardan-lib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-DV8_COMPRESS_POINTERS")

add_library(cardan-lib
    ScriptExecutionContext.cpp
    ScriptExecutionContext.hpp

    Exceptions.hpp

    Value.cpp
    Value.hpp

    Helper.hpp
)

target_include_directories(cardan-lib PUBLIC ${CMAKE_SOURCE_DIR}/v8/include)
target_link_libraries(cardan-lib PRIVATE pthread ${CMAKE_SOURCE_DIR}/v8/out.gn/x64.release.sample/obj/libv8_monolith.a ${CMAKE_DL_LIBS})
