cmake_minimum_required(VERSION 3.23)
project(sdl2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -lGL -lX11 -lpthread -lXrandr -lXi")
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

find_package(SDL2 REQUIRED SDL2)
find_package(OpenGL REQUIRED)
add_executable(sdl2 main.cpp glad.c shader.h)
target_link_libraries(sdl2 PRIVATE SDL2)
