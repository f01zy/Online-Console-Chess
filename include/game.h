#pragma once

#include <string>

class Game {
public:
  static std::string opponent;
  static std::string color;
  static bool isYourMove;
  static std::string chessboard[8][8];
  static bool isNeedToFinishAGame;

  void mode();
  void initChessboard();
  void move();
  void play();
  void menu();
  void start();
  void searchOpponent();
  void waiting();
  void readyToGame();
  void mate();
};
