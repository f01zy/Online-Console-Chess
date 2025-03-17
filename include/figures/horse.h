#pragma once

#include "figure.h"

#include <cstdlib>
#include <vector>

using namespace std;

class Horse : public Figure {
public:
  short points = 3;
  static bool validate(string board[8][8], vector<short> c) {
    if (abs(c[0] - c[2]) != 1)
      return false;

    if (abs(c[1] - c[3]) != 2)
      return false;

    return true;
  };
};
