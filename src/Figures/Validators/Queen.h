#pragma once

#include "Elephant.h"
#include "Figure.h"
#include "Rook.h"
#include <vector>

class Queen : public Figure {
public:
  short points = 9;
  static bool validate(std::string board[8][8], std::vector<short> c) {
    if (c[1] == c[3] || c[0] == c[2])
      return Rook::validate(board, c);

    if (abs(c[0] - c[2]) == abs(c[1] - c[3]))
      return Elephant::validate(board, c);

    return false;
  };
};
