#include "../include/board.h"
#include "../include/globals.h"
#include "../include/service.h"

#include <iostream>

using namespace std;

void Board::render(char board[boardHeight][boardWidth]) {
  Service service;
  service.clear();

  for (short i = 0; i < boardHeight; i++) {
    for (short j = 0; j < boardWidth; j++) {
      cout << board[i][j] << " ";
    }
    cout << endl;
  }
}
