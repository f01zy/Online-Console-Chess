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

void Game::play() {
  Board board;
  Service service;

  this->initChessboard();

  while (1) {
    service.clear();

    board.render(this->chessboard);

    if (this->isYourMove)
      this->move();

    service.sleep(1);
  }
}

void Game::initChessboard() {
  unordered_map<string, vector<string>> figures = {
      {"black",
       {"b/", "b?", "b*", "b#", "b!", "b*", "b?", "b/", "b.", "b.", "b.", "b.",
        "b.", "b.", "b.", "b."}},
      {"white",
       {"w/", "w?", "w*", "w!", "w#", "w*", "w?", "w/", "w.", "w.", "w.", "w.",
        "w.", "w.", "w.", "w."}}};

  bool isWhiteOnBottom = (color == "white");
  const string &playerColor = isWhiteOnBottom ? "white" : "black";
  const string &opponentColor = isWhiteOnBottom ? "black" : "white";

  for (short i = 0; i < boardHeight; i++) {
    for (short j = 0; j < boardWidth; j++) {
      if (i == 0) {
        chessboard[i][j] = figures[playerColor][j];
      }

      else if (i == 1) {
        chessboard[i][j] = figures[playerColor][j + 8];
      }

      else if (i == 6) {
        chessboard[i][j] = figures[opponentColor][j + 8];
      }

      else if (i == 7) {
        chessboard[i][j] = figures[opponentColor][j];
      }

      else {
        chessboard[i][j] = " ";
      }
    }
  }
}

void Game::move() {
  Socket &socket = Socket::getInstance(SERVER_URL);
  Figures figures;

  string username = Auth::user["username"];

  cout << endl;
  cout << "Coordinates: ";

  string coordinates;
  cin >> coordinates;

  if (!figures.validate(coordinates))
    return;

  Game::isYourMove = false;

  string data = username + " " + coordinates;
  socket.send("move", data);
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

  this->play();
}

void Game::searchOpponent() {
  Socket &socket = Socket::getInstance(SERVER_URL);

  string username = Auth::user["username"];
  socket.send("searchOpponent", username);

  this->waiting();
}
