#include "Figures.h"
#include "../Board/Board.h"
#include "../Game/Game.h"
#include "../Utils/Utils.h"
#include "Validators/Elephant.h"
#include "Validators/Horse.h"
#include "Validators/King.h"
#include "Validators/Pawn.h"
#include "Validators/Queen.h"
#include "Validators/Rook.h"
#include <cctype>
#include <stdexcept>

std::unordered_map<std::string,
                   std::function<bool(std::string[8][8], std::vector<short>)>>
    Figures::validateFunctions = {
        {"P", Pawn::validate},     {"K", King::validate},
        {"Q", Queen::validate},    {"R", Rook::validate},
        {"B", Elephant::validate}, {"N", Horse::validate}};

std::vector<short> Figures::getCoordinates(std::string coordinates) {
  Utils utils;

  if (!this->validateCoordinates(coordinates)) {
    return {};
  }

  std::string from = coordinates.substr(0, coordinates.find("-"));
  std::string to =
      coordinates.substr(coordinates.find("-") + 1, coordinates.size());

  short fromYStoi;
  short toYStoi;

  try {
    fromYStoi = stoi(utils.charToString(from[1]));
    toYStoi = stoi(utils.charToString(to[1]));

    if (fromYStoi < 0 || fromYStoi > 8) {
      throw std::invalid_argument("");
    }

    if (toYStoi < 0 || toYStoi > 8) {
      throw std::invalid_argument("");
    }
  }

  catch (std::invalid_argument &err) {
    return {};
  }

  short fromX = utils.getAlphabetIndex(from[0]);
  short fromY = 8 - fromYStoi;
  short toX = utils.getAlphabetIndex(to[0]);
  short toY = 8 - toYStoi;

  return {fromX, fromY, toX, toY};
}

bool Figures::isLetter(char letter) { return std::isalpha(letter); }

bool Figures::requiredMoveValidate(std::vector<short> coordinates) {
  Board &board = Board::getInstance();

  std::string fromFigure = board.chessboard[coordinates[1]][coordinates[0]];
  std::string toFigure = board.chessboard[coordinates[3]][coordinates[2]];

  if (fromFigure[0] != Game::color[0] || toFigure[0] == Game::color[0]) {
    return false;
  }

  if (this->checkCheckAfterMove(coordinates)) {
    return false;
  }

  return true;
}

bool Figures::validateCoordinates(std::string coordinates) {
  if (coordinates.size() != 5 || coordinates[2] != '-') {
    return false;
  }

  if (!isdigit(coordinates[1]) || !isdigit(coordinates[4])) {
    return false;
  }

  if (!this->isLetter(coordinates[0]) || !isLetter(coordinates[3])) {
    return false;
  }

  return true;
}

bool Figures::validateMove(std::vector<short> coordinates) {
  Board &board = Board::getInstance();

  if (coordinates.size() == 0) {
    return false;
  }

  std::string figure(1, board.chessboard[coordinates[1]][coordinates[0]][1]);
  if (figure == " ") {
    return false;
  }

  return validateFunctions[figure](board.chessboard, coordinates) &&
         requiredMoveValidate(coordinates);
}

bool Figures::check(std::string board[8][8]) {
  std::string color(1, Game::color[0]);
  std::vector<short> king = findFigure(board, color + "K");

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      std::string v = board[i][j];
      if (v == "  " || v[0] == color[0]) {
        continue;
      }

      std::string figure(1, v[1]);
      std::vector<short> move = {j, i, king[0], king[1]};

      if (validateFunctions[figure](board, move)) {
        return true;
      }
    }
  }

  return false;
}

bool Figures::checkCheckAfterMove(std::vector<short> c) {
  Board &board = Board::getInstance();
  std::string tempBoard[8][8];

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      tempBoard[i][j] = board.chessboard[i][j];
    }
  }

  tempBoard[c[1]][c[0]] = "  ";
  tempBoard[c[3]][c[2]] = board.chessboard[c[1]][c[0]];

  return check(tempBoard);
}

bool Figures::checkmate(std::string board[8][8]) {
  if (!check(board)) {
    return false;
  }

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      std::string v = board[i][j];
      if (v == "  " || v[0] != Game::color[0]) {
        continue;
      }

      std::string figure(1, v[1]);

      for (short k = 0; k < 8; k++) {
        for (short n = 0; n < 8; n++) {
          std::vector<short> coordinates = {j, i, n, k};

          if (!validateFunctions[figure](board, coordinates)) {
            continue;
          }

          if (!requiredMoveValidate(coordinates)) {
            continue;
          }

          if (!checkCheckAfterMove(coordinates)) {
            return false;
          }
        }
      }
    }
  }

  return true;
}

std::vector<short> Figures::findFigure(std::string board[8][8],
                                       std::string figure) {
  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      if (board[i][j] == figure) {
        return {j, i};
      }
    }
  }

  return {-1, -1};
}
