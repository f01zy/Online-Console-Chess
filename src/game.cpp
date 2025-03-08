#include "../include/game.h"
#include "../include/auth.h"
#include "../include/board.h"
#include "../include/figures.h"
#include "../include/globals.h"
#include "../include/service.h"
#include "../include/socket.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

string Game::opponent = "";
string Game::color = "";
bool Game::isYourMove = false;
string Game::chessboard[boardHeight][boardWidth] = {};

void Game::menu() {
  Service service;
  Auth auth;

  service.clear();

  vector<string> options = {"Online game", "Github", "Log out", "Exit"};

  for (short i = 0; i < options.size(); i++) {
    cout << options[i] << " (" << i + 1 << ")" << endl;
  }

  cout << endl;
  short choice = service.getNumber("Your choice: ");

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

void Game::play() {
  Board board;
  Service service;

  this->initChessboard();

  while (1) {
    service.clear();

    board.render();

    if (this->isYourMove)
      this->move();

    service.sleep(1);
  }
}

void Game::initChessboard() {
  unordered_map<string, vector<string>> figures = {
      {"black",
       {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR", "bP", "bP", "bP", "bP",
        "bP", "bP", "bP", "bP"}},

      {"white",
       {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR", "wP", "wP", "wP", "wP",
        "wP", "wP", "wP", "wP"}}};

  for (short j = 0; j < boardWidth; j++) {
    this->chessboard[7][j] = figures["white"][j];
    this->chessboard[6][j] = figures["white"][j + 8];
    this->chessboard[1][j] = figures["black"][j + 8];
    this->chessboard[0][j] = figures["black"][j];
  }

  for (short i = 2; i < 6; i++) {
    for (short j = 0; j < boardWidth; j++) {
      this->chessboard[i][j] = " ";
    }
  }
}

void Game::move() {
  Socket &socket = Socket::getInstance(SERVER_URL);
  Figures figures;
  Board board;
  Service service;

  string username = Auth::user["username"];

  cout << endl;
  cout << "Coordinates: ";

  string coordinates;
  cin >> coordinates;

  if (!figures.validateMove(coordinates)) {
    service.clear();
    board.render("Dont valid coordinates.");
    this->move();
  }

  board.move(coordinates);

  Game::isYourMove = false;

  string data = username + " " + coordinates;
  socket.send("move", data);
}

void Game::waiting() {
  Service service;

  short state = 1;
  char waitChar = '.';

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

    cout << "Waiting" << wait << endl;

    service.sleep(1);
  }

  this->play();
}

void Game::searchOpponent() {
  Socket &socket = Socket::getInstance(SERVER_URL);

  string username = Auth::user["username"];
  socket.send("searchOpponent", username);

  this->waiting();
}
