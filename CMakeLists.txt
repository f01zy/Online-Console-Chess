cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(sioclient REQUIRED)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB SRC_FILES "${SOURCE_DIR}/*.cpp")
add_executable(start ${SRC_FILES})

target_link_libraries(start PRIVATE CURL::libcurl)
target_link_libraries(start PRIVATE sioclient)
