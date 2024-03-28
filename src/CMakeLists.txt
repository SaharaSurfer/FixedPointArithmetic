cmake_minimum_required(VERSION 3.8)
project(FixedPointArithmetic LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

set(SRC_LIST main.cc)
include_directories("header/")

add_executable(${PROJECT_NAME} ${SRC_LIST})
