#pragma once

#include "elephant.h"
#include "figure.h"
#include "rook.h"

#include <vector>

using namespace std;

class Queen : public Figure {
public:
  short points = 9;
  static bool validate(vector<short> c) {
    if (c[1] == c[3] || c[0] == c[2])
      return Rook::validate(c);

    if (abs(c[0] - c[2]) == abs(c[1] - c[3]))
      return Elephant::validate(c);

    return false;
  };
};
