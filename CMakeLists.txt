cmake_minimum_required(VERSION 3.20.2)

project(SCTE35Parser VERSION 1.0.0.0)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(SCTE35Parser main.cpp SpliceClass.cpp)