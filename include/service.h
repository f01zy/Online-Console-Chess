#pragma once

#include <string>

using namespace std;

class Service {
public:
  short getAlphabetIndex(char letter);
  void clear();
  void sleep(short seconds);
  short getNumber(string message);
};
