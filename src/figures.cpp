#include "../include/figures.h"
#include "../include/game.h"
#include "../include/service.h"

#include <cctype>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, function<bool()>> Figures::validateFunctions = {
    {".", Figures::pawn}, {"!", Figures::king},     {"#", Figures::queen},
    {"/", Figures::rook}, {"*", Figures::elephant}, {"?", Figures::horse}};

vector<short> Figures::getCoordinates(string coordinates) {
  Service service;

  if (!this->validateCoordinates(coordinates))
    return {};

  string from = coordinates.substr(0, coordinates.find("-"));
  string to = coordinates.substr(coordinates.find("-") + 1, coordinates.size());

  return vector<short>{service.getAlphabetIndex(tolower(from[0])), from[1],
                       service.getAlphabetIndex(tolower(to[0])), to[1]};
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

  string figure(1, Game::chessboard[coordinates[0]][coordinates[1]][1]);

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
