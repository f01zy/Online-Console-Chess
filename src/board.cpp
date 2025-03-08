#include "../include/board.h"
#include "../include/figures.h"
#include "../include/game.h"
#include "../include/globals.h"
#include "../include/service.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Board::render(string error) {
  Service service;
  service.clear();

  if (error.size() > 0) {
    cout << error << endl;
    cout << endl;
  }

  vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  for (short i = 0; i < boardHeight; i++) {
    short number;
    if (Game::color == "white") {
      number = boardHeight - i;
    }

    else {
      number = i + 1;
    }

    cout << number << " ";

    for (short j = 0; j < boardWidth; j++) {
      if (Game::color == "black") {
        cout << Game::chessboard[boardHeight - i - 1][j] << " ";
      }

      else {
        cout << Game::chessboard[i][j] << " ";
      }
    }
    cout << endl;
  }

  for (short i = 0; i < letters.size(); i++) {
    short index;
    if (Game::color == "white") {
      index = i;
    }

    else {
      index = letters.size() - 1 - i;
    }

    cout << "  " << letters[index];
  }
  cout << endl;
}

void Board::move(string c) {
  Figures figures;
  vector coordinates = figures.getCoordinates(c);

  if (coordinates.size() == 0)
    return;

  string figure = Game::chessboard[coordinates[1]][coordinates[0]];

  Game::chessboard[coordinates[1]][coordinates[0]] = "  ";
  Game::chessboard[coordinates[3]][coordinates[2]] = figure;
}
