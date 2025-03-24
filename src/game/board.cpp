#include "../../include/board.h"
#include "../../include/figures.h"
#include "../../include/game.h"
#include "../../include/service.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::string> figures = {
    {"wK", "♚"}, {"wQ", "♛"}, {"wR", "♜"}, {"wN", "♞"},
    {"wB", "♝"}, {"wP", "♟"}, {"bK", "♔"}, {"bQ", "♕"},
    {"bN", "♘"}, {"bR", "♖"}, {"bP", "♙"}, {"bB", "♗"},
};

void Board::render(std::string error) {
  Service service;
  service.clear();

  std::string board[8][8];
  std::vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  if (error.size() > 0) {
    std::cout << error << std::endl;
    std::cout << std::endl;
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

    std::cout << number;
    for (short j = 0; j < 8; j++) {
      std::string figure;

      if (board[i][j] == "  ")
        figure = " ";

      else
        figure = figures.at(board[i][j]);

      std::cout << " " << figure;
    }
    std::cout << std::endl;
  }

  std::cout << " ";
  for (short i = 0; i < letters.size(); i++) {
    short index;
    if (Game::color == "white")
      index = i;

    else
      index = letters.size() - 1 - i;

    std::cout << " " << letters[index];
  }
  std::cout << std::endl;
}

void Board::move(std::string c) {
  Figures figures;
  Game game;

  std::vector coordinates = figures.getCoordinates(c);

  if (coordinates.size() == 0)
    return;

  std::string figure = Game::chessboard[coordinates[1]][coordinates[0]];

  Game::chessboard[coordinates[1]][coordinates[0]] = "  ";
  Game::chessboard[coordinates[3]][coordinates[2]] = figure;

  if (figures.checkmate(Game::chessboard))
    game.mate();
}
