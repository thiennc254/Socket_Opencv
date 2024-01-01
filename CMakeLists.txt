cmake_minimum_required(VERSION 3.0.0)
project(socket)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE client.cpp)
add_executable(socket ${SOURCE})

target_link_libraries(socket ${OpenCV_LIBS})