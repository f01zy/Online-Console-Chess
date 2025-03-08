#pragma once

#include "../include/globals.h"

#include <string>

using namespace std;

class Board {
public:
  void render(string error = "");
  void move(string c);
};
