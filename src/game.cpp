#include "../include/game.h"
#include "../include/auth.h"
#include "../include/board.h"
#include "../include/globals.h"
#include "../include/service.h"
#include "../include/socket.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string Game::opponent = "";

void Game::menu() {
  Service service;
  Auth auth;

  service.clear();

  vector<string> options = {"Online game", "Github", "Log out", "Exit"};

  for (short i = 0; i < options.size(); i++) {
    cout << options[i] << " (" << i + 1 << ")" << endl;
  }

  short choice;

  cout << endl;
  cout << "Your choice: ";
  cin >> choice;

  switch (choice) {
  case 1:
    this->searchOpponent();
    break;

  case 3:
    auth.logout();
    break;

  case 4:
    exit(0);

  default:
    this->menu();
    break;
  }

  return;
}

void Game::start() {
  Auth auth;
  Game game;

  bool isAuth = auth.refresh();

  if (!isAuth)
    auth.render();

  else
    this->menu();
}

void Game::initialization() {
  Board board;

  char chessboard[boardHeight][boardWidth] = {
      {'/', '?', '*', '#', '!', '*', '?', '/'},
      {'.', '.', '.', '.', '.', '.', '.', '.'},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {'.', '.', '.', '.', '.', '.', '.', '.'},
      {'/', '?', '*', '#', '!', '*', '?', '/'},
  };

  board.render(chessboard);
}

void Game::waiting() {
  Service service;

  short state = 1;
  char waitChar = '.';
  short i = 0;

  while (this->opponent.size() == 0) {
    service.clear();
    Socket &socket = Socket::getInstance(SERVER_URL);
    string username = Auth::user["username"];
    string wait = "";

    for (short i = 0; i < state; i++)
      wait += waitChar;

    if (state == 3)
      state = 1;
    else
      state++;

    i++;
    if (i == 5) {
      i = 0;
      socket.send("searchOpponent", username);
    }

    cout << "Waiting" << wait << endl;

    service.sleep(1);
  }

  this->initialization();
}

void Game::searchOpponent() {
  Socket &socket = Socket::getInstance(SERVER_URL);

  string username = Auth::user["username"];
  socket.send("searchOpponent", username);

  this->waiting();
}
