cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_CXX_STANDARD 17)
set(SUBDIRS src src/game src/utils)

option(WINDOWS_BUILD "Build for Windows" OFF)

if(WINDOWS_BUILD)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
endif()

find_package(CURL REQUIRED)
find_package(sioclient REQUIRED)
find_package(ftxui 5 REQUIRED)

foreach(SUBDIR ${SUBDIRS})
    file(GLOB SUBDIR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/*.cpp")
    list(APPEND SRC_FILES ${SUBDIR_SOURCES})
endforeach()

add_executable(chess ${SRC_FILES})

target_link_libraries(chess PRIVATE CURL::libcurl)
target_link_libraries(chess PRIVATE sioclient)
target_link_libraries(chess
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
