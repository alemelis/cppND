cmake_minimum_required(VERSION 2.6)
project(raytracer)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(raytracer ${SOURCES})

set_property(TARGET raytracer PROPERTY CXX_STANDARD 17)
target_compile_options(raytracer PRIVATE -Wall -Wextra)
