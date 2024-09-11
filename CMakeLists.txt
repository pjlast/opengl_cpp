cmake_minimum_required(VERSION 3.13)
project(opengl)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_subdirectory(../glfw build)

find_package(OpenGL REQUIRED)

add_subdirectory(glad)
add_subdirectory(src)
