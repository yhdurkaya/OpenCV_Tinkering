cmake_minimum_required(VERSION 3.15)
project(OpenCV_Tinkering)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)


add_executable(OpenCV_Tinkering BallTracking.cpp)

target_link_libraries( OpenCV_Tinkering ${OpenCV_LIBS})
target_link_libraries( OpenCV_Tinkering dlib::dlib)