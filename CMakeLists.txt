cmake_minimum_required(VERSION 3.26)
project(ray-tracing)

set(CMAKE_CXX_STANDARD 20)

add_executable(ray-tracing main.cpp
        Camara.cpp
        vec3.cpp
        Rayo.cpp
        Objeto.cpp
        Luz.cpp
        Luz.h)
