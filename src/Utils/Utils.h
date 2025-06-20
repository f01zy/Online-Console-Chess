#pragma once

#include <string>
#include <vector>

class Utils {
public:
  void clear();
  void sleep(double seconds);
  std::string charToString(char symbol);
  short getAlphabetIndex(char letter);
  void openPageInBrowser(std::string page);
  short menu(std::vector<std::string> options, std::string message);
  void renderWithClear();
};
