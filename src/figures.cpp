#include "../include/figures.h"
#include "../include/game.h"
#include "../include/service.h"

#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, function<bool(vector<short>)>>
    Figures::validateFunctions = {
        {"P", Figures::pawn}, {"K", Figures::king},     {"Q", Figures::queen},
        {"R", Figures::rook}, {"B", Figures::elephant}, {"N", Figures::horse}};

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

bool Figures::baseMoveValidation(vector<short> coordinates) {
  string fromFigure = Game::chessboard[coordinates[1]][coordinates[0]];
  string toFigure = Game::chessboard[coordinates[3]][coordinates[2]];

  if (fromFigure[0] != Game::color[0] || toFigure[0] == Game::color[0])
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

  return this->baseMoveValidation(coordinates) &&
         this->validateFunctions[figure](coordinates);
}

bool Figures::pawn(vector<short> c) {
  char opponentColor = Game::color == "white" ? 'b' : 'w';

  short maxAdvance = (c[1] == 6 || c[1] == 1) ? 2 : 1;
  short direction = (opponentColor == 'b') ? 1 : -1;

  if ((c[1] - c[3]) * direction > maxAdvance) {
    return false;
  }

  if (abs(c[0] - c[2]) > 1 ||
      (c[0] - c[2] != 0 && Game::chessboard[c[3]][c[2]][0] != opponentColor)) {
    return false;
  }

  return true;
}

bool Figures::king(vector<short> c) {
  if (abs(c[1] - c[3]) > 1 || abs(c[0] - c[2]) > 1)
    return false;

  return true;
}

bool Figures::queen(vector<short> c) { return true; }

bool Figures::horse(vector<short> c) {
  if (abs(c[0] - c[2]) != 1)
    return false;

  if (abs(c[1] - c[3]) != 2)
    return false;

  return true;
}

bool Figures::rook(vector<short> c) {
  if (c[1] != c[3] && c[0] != c[2])
    return false;

  short dirX = (c[2] - c[0]) == 0 ? 0 : (c[2] - c[0]) > 0 ? 1 : -1;
  short dirY = (c[3] - c[1]) == 0 ? 0 : (c[3] - c[1]) > 0 ? 1 : -1;

  short x = c[0] + dirX;
  short y = c[1] + dirY;

  while (x != c[2] || y != c[3]) {
    if (Game::chessboard[y][x] != "  ") {
      return false;
    }

    x += dirX;
    y += dirY;
  }

  return true;
}

bool Figures::elephant(vector<short> c) { return true; }
