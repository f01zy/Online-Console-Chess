#include "../include/board.h"
#include "../include/figures.h"
#include "../include/game.h"
#include "../include/service.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const unordered_map<string, string> figures = {
    {"wK", "♚"}, {"wQ", "♛"}, {"wR", "♜"}, {"wN", "♞"},
    {"wB", "♝"}, {"wP", "♟"}, {"bK", "♔"}, {"bQ", "♕"},
    {"bN", "♘"}, {"bR", "♖"}, {"bP", "♙"}, {"bB", "♗"},
};

void Board::render(string error) {
  Service service;
  service.clear();

  string board[8][8];
  vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  if (error.size() > 0) {
    cout << error << endl;
    cout << endl;
  }

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      board[i][j] = Game::chessboard[i][j];
    }
  }

  if (Game::color == "black") {
    for (short i = 0; i < 8 / 2; ++i) {
      swap(board[i], board[8 - i - 1]);
    }

    for (short i = 0; i < 8; ++i) {
      reverse(board[i], board[i] + 8);
    }
  }

  for (short i = 0; i < 8; i++) {
    short number = 8 - i;

    if (Game::color == "black")
      number = i + 1;

    cout << number;
    for (short j = 0; j < 8; j++) {
      string figure;

      if (board[i][j] == "  ")
        figure = " ";

      else
        figure = figures.at(board[i][j]);

      cout << " " << figure;
    }
    cout << endl;
  }

  cout << " ";
  for (short i = 0; i < letters.size(); i++) {
    short index;
    if (Game::color == "white")
      index = i;

    else
      index = letters.size() - 1 - i;

    cout << " " << letters[index];
  }
  cout << endl;
}

void Board::move(string c) {
  Figures figures;
  Game game;

  vector coordinates = figures.getCoordinates(c);

  if (coordinates.size() == 0)
    return;

  string figure = Game::chessboard[coordinates[1]][coordinates[0]];

  Game::chessboard[coordinates[1]][coordinates[0]] = "  ";
  Game::chessboard[coordinates[3]][coordinates[2]] = figure;

  if (figures.checkmate(Game::chessboard))
    game.mate();
}
