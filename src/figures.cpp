#include "../include/figures.h"
#include "../include/game.h"
#include "../include/service.h"

#include <cctype>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, function<bool()>> Figures::validateFunctions = {
    {".", Figures::pawn}, {"!", Figures::king},     {"#", Figures::queen},
    {"/", Figures::rook}, {"*", Figures::elephant}, {"?", Figures::horse}};

vector<string> Figures::getCoordinates(string coordinates) {
  if (!this->validateCoordinates(coordinates))
    return {};

  string from = coordinates.substr(0, coordinates.find("-"));
  string to = coordinates.substr(coordinates.find("-") + 1, coordinates.size());

  return vector<string>{from, to};
}

bool Figures::validateCoordinates(string coordinates) {
  if (coordinates.size() != 5 || coordinates[2] != '-' ||
      !isdigit(coordinates[1]) || !isdigit(coordinates[4]))
    return false;

  return true;
}

bool Figures::validateMove(string c) {
  Service service;

  vector<string> coordinates = this->getCoordinates(c);

  if (coordinates.size() == 0)
    return false;

  string from = coordinates[0];
  string to = coordinates[1];

  short y = service.getAlphabetIndex(from[0]);
  string figure(1, Game::chessboard[y][to[1]][1]);

  if (this->validateFunctions[figure])
    return this->validateFunctions[figure]();

  else
    return false;
}

bool Figures::pawn() { return true; }
bool Figures::king() { return true; }
bool Figures::queen() { return true; }
bool Figures::horse() { return true; }
bool Figures::rook() { return true; }
bool Figures::elephant() { return true; }
