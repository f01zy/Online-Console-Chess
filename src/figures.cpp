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

unordered_map<string, function<bool(vector<short>)>>
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

    if (fromYStoi < 0 || fromYStoi > boardHeight)
      throw invalid_argument("");

    if (toYStoi < 0 || toYStoi > boardHeight)
      throw invalid_argument("");
  }

  catch (invalid_argument &err) {
    return {};
  }

  short fromX = service.getAlphabetIndex(from[0]);
  short fromY = boardHeight - fromYStoi;
  short toX = service.getAlphabetIndex(to[0]);
  short toY = boardHeight - toYStoi;

  return {fromX, fromY, toX, toY};
}

bool Figures::isLetter(char letter) { return std::isalpha(letter); }

bool Figures::requiredMoveValidate(vector<short> coordinates) {
  string fromFigure = Game::chessboard[coordinates[1]][coordinates[0]];
  string toFigure = Game::chessboard[coordinates[3]][coordinates[2]];

  if (fromFigure[0] != Game::color[0] || toFigure[0] == Game::color[0])
    return false;

  if (this->check())
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

bool Figures::validateMove(string c) {
  Service service;

  vector<short> coordinates = this->getCoordinates(c);

  if (coordinates.size() == 0)
    return false;

  string figure(1, Game::chessboard[coordinates[1]][coordinates[0]][1]);

  return this->requiredMoveValidate(coordinates) &&
         this->validateFunctions[figure](coordinates);
}

bool Figures::check() {
  string color(1, Game::color[0]);
  vector<short> king = this->findFigure(color + "K");

  for (short i = 0; i < boardHeight; i++) {
    for (short j = 0; j < boardWidth; j++) {
      string v = Game::chessboard[i][j];

      if (v == "  " || v[0] == color[0])
        continue;

      string figure(1, v[1]);
      vector<short> move = {j, i, king[0], king[1]};

      if (this->validateFunctions[figure](move)) {
        return true;
      }
    }
  }

  return false;
}

vector<short> Figures::findFigure(string figure) {
  for (short i = 0; i < boardHeight; i++)
    for (short j = 0; j < boardWidth; j++)
      if (Game::chessboard[i][j] == figure)
        return {j, i};

  return {-1, -1};
}
