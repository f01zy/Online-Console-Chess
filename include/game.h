#pragma once

#include <string>

using namespace std;

class Game {
public:
  static string opponent;

  void chess();
  void menu();
  void start();
  void searchOpponent();
  void waiting();
};
