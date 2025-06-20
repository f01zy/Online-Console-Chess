cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_CXX_STANDARD 17)
set(FILES
    ./src/main.cpp
    ./src/globals.cpp
    ./src/Auth/Auth.cpp
    ./src/Game/Game.cpp
    ./src/Board/Board.cpp
    ./src/Utils/Utils.cpp
    ./src/Figures/Figures.cpp
    ./src/Network/Http.cpp
    ./src/Network/Socket.cpp)

add_executable(chess ${FILES})

find_package(CURL REQUIRED)
find_package(sioclient REQUIRED)
find_package(ftxui REQUIRED)

target_link_libraries(chess PRIVATE CURL::libcurl)
target_link_libraries(chess PRIVATE sioclient)
target_link_libraries(
  chess
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component)
