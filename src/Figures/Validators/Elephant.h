#pragma once

#include "Figure.h"
#include <vector>

class Elephant : public Figure {
public:
  short points = 3;
  static bool validate(std::string board[8][8], std::vector<short> c) {
    if (abs(c[0] - c[2]) != abs(c[1] - c[3])) {
      return false;
    }

    short dirX = (c[2] - c[0]) > 0 ? 1 : -1;
    short dirY = (c[3] - c[1]) > 0 ? 1 : -1;

    short x = c[0] + dirX;
    short y = c[1] + dirY;

    while (x != c[2] && y != c[3]) {
      if (board[y][x] != "  ") {
        return false;
      }

      x += dirX;
      y += dirY;
    }

    return true;
  };
};
