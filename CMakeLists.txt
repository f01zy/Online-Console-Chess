cmake_minimum_required(VERSION 3.10)
project(chess)
set(CMAKE_CXX_STANDARD 17)

option(WINDOWS_BUILD "Build for Windows" OFF)

if(WINDOWS_BUILD)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
endif()

find_package(CURL REQUIRED)
find_package(sioclient REQUIRED)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB SRC_FILES "${SOURCE_DIR}/*.cpp")
add_executable(chess ${SRC_FILES})

target_link_libraries(chess PRIVATE CURL::libcurl)
target_link_libraries(chess PRIVATE sioclient)
