#include "../include/board.h"
#include "../include/game.h"
#include "../include/globals.h"
#include "../include/service.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Board::render(string board[boardHeight][boardWidth]) {
  Service service;
  service.clear();

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
        cout << board[boardHeight - i - 1][j] << " ";
      }

      else {
        cout << board[i][j] << " ";
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
