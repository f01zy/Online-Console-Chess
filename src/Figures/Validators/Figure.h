#pragma once

#include <string>
#include <vector>

class Figure {
public:
  short points;
  static bool validate(std::string board[8][8], std::vector<short> c);
};
