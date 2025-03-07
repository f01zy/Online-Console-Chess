#include "../include/figures.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, function<bool()>> Figures::validateFunctions = {
    {".", Figures::pawn}, {"!", Figures::king},     {"#", Figures::queen},
    {"/", Figures::rook}, {"*", Figures::elephant}, {"?", Figures::horse}};

vector<string> Figures::getCoordinates(string coordinates) {
  string from = coordinates.substr(0, coordinates.find("-"));
  string to = coordinates.substr(coordinates.find("-"), coordinates.size());

  return vector<string>{from, to};
}

bool Figures::validate(string c) {
  vector<string> coordinates = this->getCoordinates(c);
  string figure(1, coordinates[0][0]);

  return this->validateFunctions[figure]();
}

bool Figures::pawn() { return true; }
bool Figures::king() { return true; }
bool Figures::queen() { return true; }
bool Figures::horse() { return true; }
bool Figures::rook() { return true; }
bool Figures::elephant() { return true; }
