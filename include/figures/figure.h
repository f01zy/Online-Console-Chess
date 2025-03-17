#pragma once

#include "../game.h"

#include <string>
#include <vector>

using namespace std;

class Figure {
public:
  short points;
  static bool validate(string board[8][8], vector<short> c);
};
