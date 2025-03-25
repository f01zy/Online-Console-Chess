#include "../../include/board.h"
#include "../../include/figures.h"
#include "../../include/game.h"
#include "../../include/service.h"

#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

using namespace ftxui;

const std::unordered_map<std::string, std::string> figures = {
    {"wK", "♚"}, {"wQ", "♛"}, {"wR", "♜"}, {"wN", "♞"},
    {"wB", "♝"}, {"wP", "♟"}, {"bK", "♔"}, {"bQ", "♕"},
    {"bN", "♘"}, {"bR", "♖"}, {"bP", "♙"}, {"bB", "♗"},
};

std::vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

void Board::render(std::string error) {
  std::string board[8][8];
  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      board[i][j] = Game::chessboard[i][j];
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
           bgcolor(isWhite ? Color::RGB(0, 0, 0) : Color::RGB(41, 41, 41));
  };

  auto createLetterRow = [this]() {
    Elements lettersElements = {text("   ")};
    for (short i = 0; i < letters.size(); i++) {
      short index = (Game::color == "white") ? i : letters.size() - 1 - i;
      lettersElements.push_back(
          text(" " + std::string(1, letters[index]) + " ") | center);
    }

    return hbox(lettersElements);
  };

  Elements rows;
  for (short i = 0; i < 8; i++) {
    short number = (Game::color == "black") ? i + 1 : 8 - i;
    Elements rowElements;

    rowElements.push_back(text(" " + std::to_string(number) + " ") | center);

    for (short j = 0; j < 8; j++) {
      std::string figure =
          (board[i][j] == "  ") ? " " : figures.at(board[i][j]);
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
                    : vbox({text(error) | color(Color::Red) | center,
                            filler() | size(HEIGHT, EQUAL, 1), boardDocument}),
      filler() | size(HEIGHT, EQUAL, 1))));

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}

void Board::move(std::string c) {
  Figures figures;
  Game game;
  Service service;

  std::vector coordinates = figures.getCoordinates(c);

  if (coordinates.size() == 0)
    return;

  std::string figure = Game::chessboard[coordinates[1]][coordinates[0]];

  Game::chessboard[coordinates[1]][coordinates[0]] = "  ";
  Game::chessboard[coordinates[3]][coordinates[2]] = figure;

  if (figures.checkmate(Game::chessboard))
    game.mate();

  service.renderWithClear();
}
