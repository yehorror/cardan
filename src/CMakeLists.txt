cmake_minimum_required(VERSION 3.5)

project(cardan-lib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-DV8_COMPRESS_POINTERS")

add_library(cardan-lib
    Context.cpp
    Context.hpp
    #Context.inl

    Exceptions.hpp

    Value/Value.cpp
    Value/Value.hpp

    Value/Array.cpp
    Value/Array.hpp

    Value/Object.cpp
    Value/Object.hpp

    Value/Function.cpp
    Value/Function.hpp

    Converters/ToV8.cpp
    Converters/ToV8.hpp

    Converters/FromV8.cpp
    Converters/FromV8.hpp

    Helper.hpp
)

target_include_directories(cardan-lib PUBLIC ${CMAKE_SOURCE_DIR}/v8/include)
target_include_directories(cardan-lib PRIVATE .)
target_link_libraries(cardan-lib PRIVATE pthread ${CMAKE_SOURCE_DIR}/v8/out.gn/x64.release.sample/obj/libv8_monolith.a ${CMAKE_DL_LIBS})
