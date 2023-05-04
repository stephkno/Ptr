cmake_minimum_required(VERSION 3.0)
project(test)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "-w -g")

add_executable(test main.cc)
include_directories(.)
