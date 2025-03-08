#include "../include/figures.h"
#include "../include/game.h"
#include "../include/service.h"

#include <cctype>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, function<bool()>> Figures::validateFunctions = {
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

bool Figures::baseMoveValidation(vector<short> coordinates) { return true; }

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

  if (this->baseMoveValidation(coordinates) &&
      this->validateFunctions[figure]())
    return true;

  else
    return false;
}

bool Figures::pawn() { return true; }
bool Figures::king() { return true; }
bool Figures::queen() { return true; }
bool Figures::horse() { return true; }
bool Figures::rook() { return true; }
bool Figures::elephant() { return true; }
