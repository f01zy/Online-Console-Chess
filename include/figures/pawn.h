#pragma once

#include "figure.h"

#include <vector>

using namespace std;

class Pawn : public Figure {
public:
  short points = 1;
  static bool validate(string board[8][8], vector<short> c) {
    char opponentColor = Game::color == "white" ? 'b' : 'w';

    short maxAdvance = (c[1] == 6 || c[1] == 1) ? 2 : 1;
    short direction = (opponentColor == 'b') ? 1 : -1;

    if ((c[1] - c[3]) * direction < 0)
      return false;

    if ((c[1] - c[3]) * direction > maxAdvance)
      return false;

    if (abs(c[0] - c[2]) > 1)
      return false;

    if (c[0] != c[2] && board[c[3]][c[2]][0] != opponentColor)
      return false;

    if (c[0] != c[2] && abs(c[1] - c[3]) == 1)
      return true;

    if (c[0] == c[2] && abs(c[1] - c[3]) > 0)
      return true;

    return false;
  };
};
