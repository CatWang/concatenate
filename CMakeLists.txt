cmake_minimum_required(VERSION 2.8)
project(concatecate)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(concatecate ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(concatecate ${OpenCV_LIBS} )