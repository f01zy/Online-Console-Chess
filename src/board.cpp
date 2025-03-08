#include "../include/board.h"
#include "../include/figures.h"
#include "../include/game.h"
#include "../include/globals.h"
#include "../include/service.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Board::render(string error) {
  Service service;
  service.clear();

  string board[boardHeight][boardWidth];
  vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  if (error.size() > 0) {
    cout << error << endl;
    cout << endl;
  }

  for (short i = 0; i < boardHeight; i++) {
    for (short j = 0; j < boardWidth; j++) {
      board[i][j] = Game::chessboard[i][j];
    }
  }

  if (Game::color == "black") {
    for (short i = 0; i < boardHeight / 2; ++i) {
      swap(board[i], board[boardHeight - i - 1]);
    }

    for (short i = 0; i < boardHeight; ++i) {
      reverse(board[i], board[i] + boardWidth);
    }
  }

  for (short i = 0; i < boardHeight; i++) {
    short number = boardHeight - i;

    if (Game::color == "black")
      number = i + 1;

    cout << number;
    for (short j = 0; j < boardWidth; j++) {
      cout << " " << board[i][j];
    }
    cout << endl;
  }

  for (short i = 0; i < letters.size(); i++) {
    short index;
    if (Game::color == "white")
      index = i;

    else
      index = letters.size() - 1 - i;

    cout << "  " << letters[index];
  }
  cout << endl;
}

void Board::move(string c) {
  Figures figures;
  vector coordinates = figures.getCoordinates(c);

  if (coordinates.size() == 0)
    return;

  string figure = Game::chessboard[coordinates[1]][coordinates[0]];

  Game::chessboard[coordinates[1]][coordinates[0]] = "  ";
  Game::chessboard[coordinates[3]][coordinates[2]] = figure;
}
