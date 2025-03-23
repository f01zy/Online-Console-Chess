#pragma once

#include <string>
#include <vector>

class Service {
public:
  std::string charToString(char symbol);
  short getAlphabetIndex(char letter);
  void clear();
  void sleep(short seconds);
  short getNumber(std::string message);
  void openPageInBrowser(std::string page);
  short select(std::vector<std::string> options);
};
