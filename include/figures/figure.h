#pragma once

#include <vector>

using namespace std;

class Figure {
public:
  short points;
  static bool validate(vector<short> c);
};
