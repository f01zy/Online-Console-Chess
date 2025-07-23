#include "Board.h"
#include "../Figures/Figures.h"
#include "../Game/Game.h"
#include "../Utils/Utils.h"
#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <unordered_map>
#include <vector>

using namespace ftxui;

std::vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const std::unordered_map<std::string, std::string> figures = {
    {"wK", "♚"}, {"wQ", "♛"}, {"wR", "♜"}, {"wN", "♞"},
    {"wB", "♝"}, {"wP", "♟"}, {"bK", "♔"}, {"bQ", "♕"},
    {"bN", "♘"}, {"bR", "♖"}, {"bP", "♙"}, {"bB", "♗"},
};

Board::Board() { clear(); }

void Board::clear() {
  std::string board[8][8] = {
      {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
      {"bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
      {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"},
  };

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      chessboard[i][j] = board[i][j];
    }
  }
}

Board &Board::getInstance() {
  static Board instance;
  return instance;
}

void Board::render(std::string error) {
  std::string board[8][8];

  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      board[i][j] = chessboard[i][j];
    }
  }

  if (Game::color == "black") {
    for (short i = 0; i < 8 / 2; ++i) {
      std::swap(board[i], board[8 - i - 1]);
    }

    for (short i = 0; i < 8; ++i) {
      std::reverse(board[i], board[i] + 8);
    }
  }

  auto createCell = [](const std::string &content, bool isWhite) {
    return text(content) | size(WIDTH, EQUAL, 3) | center |
           bgcolor(isWhite ? Color::RGB(0, 0, 0) : Color::RGB(42, 42, 42));
  };

  auto createLetterRow = [this]() {
    Elements lettersElements = {text("   ")};

    for (short i = 0; i < letters.size(); i++) {
      short index = Game::color == "white" ? i : letters.size() - 1 - i;
      lettersElements.push_back(
          text(" " + std::string(1, letters[index]) + " ") | center);
    }

    return hbox(lettersElements);
  };

  Elements rows;
  for (short i = 0; i < 8; i++) {
    short number = Game::color == "black" ? i + 1 : 8 - i;

    Elements rowElements;
    rowElements.push_back(text(" " + std::to_string(number) + " ") | center);

    for (short j = 0; j < 8; j++) {
      std::string figure = board[i][j] == "  " ? " " : figures.at(board[i][j]);
      bool isWhiteCell = (i + j) % 2 == 0;
      rowElements.push_back(createCell(" " + figure + " ", isWhiteCell));
    }

    rowElements.push_back(text(" " + std::to_string(number) + " ") | center);
    rows.push_back(hbox(rowElements));
  }

  auto boardDocument = vbox({createLetterRow(), vbox(rows), createLetterRow()});
  auto document = center(vcenter(vbox(
      filler() | size(HEIGHT, EQUAL, 1),
      text("Your opponent: " + Game::opponent) | bold | dim,
      filler() | size(HEIGHT, EQUAL, 1),
      error.empty() ? boardDocument
                    : vbox({text(error) | color(Color::Red),
                            filler() | size(HEIGHT, EQUAL, 1), boardDocument}),
      filler() | size(HEIGHT, EQUAL, 1))));

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}

void Board::move(std::string c) {
  Figures figures;
  Game game;
  Utils utils;

  std::vector coordinates = figures.getCoordinates(c);

  if (coordinates.size() == 0) {
    return;
  }

  std::string figure = chessboard[coordinates[1]][coordinates[0]];
  chessboard[coordinates[1]][coordinates[0]] = "  ";
  chessboard[coordinates[3]][coordinates[2]] = figure;

  if (figures.checkmate(chessboard)) {
    game.mate();
  }

  utils.renderWithClear();
}
