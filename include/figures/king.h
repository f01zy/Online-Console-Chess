#pragma once

#include "figure.h"

#include <cstdlib>
#include <vector>

class King : public Figure {
public:
  short points;
  static bool validate(std::string board[8][8], std::vector<short> c) {
    if (abs(c[1] - c[3]) > 1 || abs(c[0] - c[2]) > 1)
      return false;

    return true;
  };
};
