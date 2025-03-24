#pragma once

#include "ftxui/component/screen_interactive.hpp"

#include <string>
#include <vector>

using namespace ftxui;

class Service {
public:
  void clear();
  void sleep(short seconds);
  std::string charToString(char symbol);
  short getAlphabetIndex(char letter);
  void openPageInBrowser(std::string page);
  short menu(std::vector<std::string> options, std::string message);
};
