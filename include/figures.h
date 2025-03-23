#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class Figures {
public:
  static std::unordered_map<
      std::string, std::function<bool(std::string[8][8], std::vector<short>)>>
      validateFunctions;

  std::vector<short> getCoordinates(std::string coordinates);
  bool isLetter(char letter);
  std::vector<short> findFigure(std::string board[8][8], std::string figure);

  bool validateMove(std::vector<short> coordinates);
  bool validateCoordinates(std::string coordinates);
  bool requiredMoveValidate(std::vector<short> coordinates);

  bool check(std::string board[8][8]);
  bool checkCheckAfterMove(std::vector<short> c);
  bool checkmate(std::string board[8][8]);
};
