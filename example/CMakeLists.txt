cmake_minimum_required(VERSION 3.9.0)
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(yukihttp_example VERSION 1.0.0 LANGUAGES CXX)

add_executable(example main.cpp)
target_compile_features(example PRIVATE cxx_std_17)
target_compile_options(example PRIVATE -Wall -Wextra -Wpedantic -Wunused-parameter -Wunreachable-code)


find_package(yukiHTTP 1.0 REQUIRED)
target_link_libraries(example yukiHTTP::yukiHTTP)

