cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(sioclient REQUIRED)
add_executable(start main.cpp)

target_link_libraries(start PRIVATE sioclient)
