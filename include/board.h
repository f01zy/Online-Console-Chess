#pragma once

#include <string>

using namespace std;

class Board {
public:
  void render(string error = "");
  void move(string c);
};
