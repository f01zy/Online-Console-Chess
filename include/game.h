#pragma once

#include <string>

using namespace std;

class Game {
public:
  static string opponent;
  static string color;
  static bool isYourMove;
  static string chessboard[8][8];
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
