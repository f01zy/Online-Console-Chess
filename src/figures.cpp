#include "../include/figures.h"
#include "../include/game.h"
#include "../include/service.h"

#include "../include/figures/elephant.h"
#include "../include/figures/horse.h"
#include "../include/figures/king.h"
#include "../include/figures/pawn.h"
#include "../include/figures/queen.h"
#include "../include/figures/rook.h"

#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, function<bool(string[8][8], vector<short>)>>
    Figures::validateFunctions = {
        {"P", Pawn::validate},     {"K", King::validate},
        {"Q", Queen::validate},    {"R", Rook::validate},
        {"B", Elephant::validate}, {"N", Horse::validate}};

vector<short> Figures::getCoordinates(string coordinates) {
  Service service;

  if (!this->validateCoordinates(coordinates))
    return {};

  string from = coordinates.substr(0, coordinates.find("-"));
  string to = coordinates.substr(coordinates.find("-") + 1, coordinates.size());

  short fromYStoi;
  short toYStoi;

  try {
    fromYStoi = stoi(service.charToString(from[1]));
    toYStoi = stoi(service.charToString(to[1]));

    if (fromYStoi < 0 || fromYStoi > 8)
      throw invalid_argument("");

    if (toYStoi < 0 || toYStoi > 8)
      throw invalid_argument("");
  }

  catch (invalid_argument &err) {
    return {};
  }

  short fromX = service.getAlphabetIndex(from[0]);
  short fromY = 8 - fromYStoi;
  short toX = service.getAlphabetIndex(to[0]);
  short toY = 8 - toYStoi;

  return {fromX, fromY, toX, toY};
}

bool Figures::isLetter(char letter) { return std::isalpha(letter); }

bool Figures::requiredMoveValidate(vector<short> coordinates) {
  Game game;

  string fromFigure = Game::chessboard[coordinates[1]][coordinates[0]];
  string toFigure = Game::chessboard[coordinates[3]][coordinates[2]];

  if (fromFigure[0] != Game::color[0] || toFigure[0] == Game::color[0])
    return false;

  if (this->checkCheckAfterMove(coordinates))
    return false;

  return true;
}

bool Figures::validateCoordinates(string coordinates) {
  if (coordinates.size() != 5 || coordinates[2] != '-')
    return false;

  if (!isdigit(coordinates[1]) || !isdigit(coordinates[4]))
    return false;

  if (!this->isLetter(coordinates[0]) || !this->isLetter(coordinates[3]))
    return false;

  return true;
}

bool Figures::validateMove(vector<short> coordinates) {
  Service service;

  if (coordinates.size() == 0)
    return false;

  string figure(1, Game::chessboard[coordinates[1]][coordinates[0]][1]);

  return this->validateFunctions[figure](Game::chessboard, coordinates) &&
         this->requiredMoveValidate(coordinates);
}

bool Figures::check(string board[8][8]) {
  string color(1, Game::color[0]);
  vector<short> king = this->findFigure(board, color + "K");

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      string v = board[i][j];

      if (v == "  " || v[0] == color[0])
        continue;

      string figure(1, v[1]);
      vector<short> move = {j, i, king[0], king[1]};

      if (this->validateFunctions[figure](board, move)) {
        return true;
      }
    }
  }

  return false;
}

bool Figures::checkCheckAfterMove(vector<short> c) {
  Service service;

  string tempBoard[8][8];

  for (short i = 0; i < 8; i++)
    for (short j = 0; j < 8; j++)
      tempBoard[i][j] = Game::chessboard[i][j];

  tempBoard[c[1]][c[0]] = "  ";
  tempBoard[c[3]][c[2]] = Game::chessboard[c[1]][c[0]];

  return this->check(tempBoard);
}

bool Figures::checkmate(string board[8][8]) {
  Service service;

  if (!this->check(board)) {
    return false;
  }

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      string v = board[i][j];

      if (v == "  " || v[0] != Game::color[0])
        continue;

      string figure(1, v[1]);

      for (short k = 0; k < 8; k++) {
        for (short n = 0; n < 8; n++) {
          vector<short> coordinates = {j, i, n, k};

          if (!this->validateFunctions[figure](board, coordinates))
            continue;

          if (!this->requiredMoveValidate(coordinates))
            continue;

          if (!this->checkCheckAfterMove(coordinates))
            return false;
        }
      }
    }
  }

  return true;
}

vector<short> Figures::findFigure(string board[8][8], string figure) {
  for (short i = 0; i < 8; i++)
    for (short j = 0; j < 8; j++)
      if (board[i][j] == figure)
        return {j, i};

  return {-1, -1};
}
