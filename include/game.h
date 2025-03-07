#pragma once

#include <string>

using namespace std;

class Game {
public:
  static string opponent;
  static string color;
  static bool isYourMove;

  void move();
  void play();
  void menu();
  void start();
  void searchOpponent();
  void waiting();
  void readyToGame();
};
