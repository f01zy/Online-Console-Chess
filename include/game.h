#pragma once

#include "../include/globals.h"

#include <string>

using namespace std;

class Game {
public:
  static string opponent;
  static string color;
  static bool isYourMove;
  static string chessboard[boardHeight][boardWidth];

  void initChessboard();
  void move();
  void play();
  void menu();
  void start();
  void searchOpponent();
  void waiting();
  void readyToGame();
};
