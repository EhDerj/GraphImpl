cmake_minimum_required(VERSION 3.21)
project(GraphImpl)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable( GraphImpl
        main.cpp
        graphs.cpp
        graphs.h
        graphFactory.h)
