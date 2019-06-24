cmake_minimum_required(VERSION 3.5)
project(faceAnal)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenCV REQUIRED)

add_executable(faceCV main.cpp)
target_link_libraries(faceCV ${OpenCV_LIBS})
