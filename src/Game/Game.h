#pragma once

#include <string>

class Game {
public:
  static bool isYourMove;
  static bool isNeedToFinishAGame;
  static std::string opponent;
  static std::string color;

  void mode();
  void move();
  void play();
  void menu();
  void start();
  void searchOpponent();
  void readyToGame();
  void waiting();
  void mate();
  void profile();
};
