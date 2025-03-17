#pragma once

#include "globals.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Figures {
public:
  static unordered_map<string, function<bool(vector<short>)>> validateFunctions;

  vector<short> getCoordinates(string coordinates);
  bool isLetter(char letter);
  vector<short> findFigure(string figure);

  bool validateMove(string coordinates);
  bool validateCoordinates(string coordinates);
  bool requiredMoveValidate(vector<short> coordinates);

  bool check();
};
