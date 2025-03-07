#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Figures {
public:
  static unordered_map<string, function<bool()>> validateFunctions;

  vector<string> getCoordinates(string coordinates);
  bool validateMove(string coordinates);
  bool validateCoordinates(string coordinates);
  static bool pawn();
  static bool king();
  static bool queen();
  static bool horse();
  static bool elephant();
  static bool rook();
};
