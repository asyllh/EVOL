cmake_minimum_required(VERSION 3.8)
project(EVOL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp methods.cpp methods.h base.cpp base.h)
add_executable(EVOL ${SOURCE_FILES})