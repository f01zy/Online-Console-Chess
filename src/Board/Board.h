#pragma once

#include <string>

class Board {
public:
  Board();
  Board(Board &) = delete;

  static Board &getInstance();
  void clear();
  void render(std::string error = "");
  void move(std::string c);

  std::string chessboard[8][8];
};
