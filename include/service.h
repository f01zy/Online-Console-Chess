#pragma once

#include <string>
#include <vector>

using namespace std;

class Service {
public:
  string charToString(char symbol);
  short getAlphabetIndex(char letter);
  void clear();
  void sleep(short seconds);
  short getNumber(string message);
  void openPageInBrowser(string page);
  short select(vector<string> options);
};
