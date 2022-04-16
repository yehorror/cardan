cmake_minimum_required(VERSION 3.5)

project(Cardan)

# TODO this should be fixed
set(CARDAN_V8_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/v8/include")
set(CARDAN_V8_LIBRARY "${CMAKE_SOURCE_DIR}/v8/out.gn/x64.release.sample/obj/libv8_monolith.a")

add_subdirectory(src)
add_subdirectory(test)
