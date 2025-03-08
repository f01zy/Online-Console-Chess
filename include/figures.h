#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Figures {
public:
  static unordered_map<string, function<bool()>> validateFunctions;

  vector<short> getCoordinates(string coordinates);
  bool isLetter(char letter);

  bool validateMove(string coordinates);
  bool validateCoordinates(string coordinates);
  bool baseMoveValidation(vector<short> coordinates);

  static bool pawn();
  static bool king();
  static bool queen();
  static bool horse();
  static bool elephant();
  static bool rook();
};
