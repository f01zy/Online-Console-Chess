#pragma once

#include <string>

class Board {
public:
  void printLetters();
  void render(std::string error = "");
  void move(std::string c);
};
