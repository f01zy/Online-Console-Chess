#include "../../include/game.h"
#include "../../include/auth.h"
#include "../../include/board.h"
#include "../../include/figures.h"
#include "../../include/globals.h"
#include "../../include/service.h"
#include "../../include/socket.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <cstdlib>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace ftxui;

std::string Game::opponent = "";
std::string Game::color = "";
bool Game::isYourMove = false;
std::string Game::chessboard[8][8] = {};
bool Game::isNeedToFinishAGame = false;

void Game::mode() {
  Service service;
  service.clear();

  std::vector<std::string> options = {"Online", "Local"};
  short choice = service.menu(options, "Game mode");

  switch (choice) {
  case 0:
    setApiUrl(PRODUCTION_URL);
    break;

  case 1:
    setApiUrl(DEVELOPMENT_URL);
    break;

  default:
    this->mode();
    break;
  }
}

void Game::menu() {
  Service service;
  Auth auth;
  Socket &socket = Socket::getInstance();

  service.clear();

  std::vector<std::string> options = {"Online game", "Profile", "Github",
                                      "Log out", "Exit"};
  short choice = service.menu(options, "Menu");

  switch (choice) {
  case 0:
    this->searchOpponent();
    break;

  case 1:
    this->profile();
    break;

  case 2:
    service.openPageInBrowser(GITHUB);
    this->menu();
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

  std::string isAuth = auth.refresh();

  if (isAuth.size() > 0)
    auth.render();

  else
    this->menu();
}

void Game::play() {
  Board board;
  Service service;

  this->initChessboard();

  while (!this->isNeedToFinishAGame) {
    service.clear();

    board.render();

    if (this->isYourMove)
      this->move();

    service.sleep(1);
  }

  this->isNeedToFinishAGame = false;
  this->isYourMove = false;
  this->opponent.clear();
  this->color.clear();

  this->menu();
}

void Game::initChessboard() {
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

  for (short i = 0; i < 8; i++)
    for (short j = 0; j < 8; j++)
      this->chessboard[i][j] = board[i][j];
}

void Game::move() {
  Socket &socket = Socket::getInstance();
  Figures figures;
  Board board;
  Service service;

  std::string username = Auth::user["username"];

  std::cout << std::endl;
  std::cout << "Coordinates: ";

  std::string coordinates;
  std::cin >> coordinates;

  if (!figures.validateMove(figures.getCoordinates(coordinates))) {
    service.clear();
    board.render("Dont valid coordinates.");
    return this->move();
  }

  board.move(coordinates);

  Game::isYourMove = false;

  std::string data = username + " " + coordinates;
  socket.send("move", data);
}

void Game::waiting() {
  Service service;

  short state = 1;
  char waitChar = '.';

  while (this->opponent.size() == 0) {
    service.clear();
    Socket &socket = Socket::getInstance();
    std::string username = Auth::user["username"];
    std::string wait = "";

    for (short i = 0; i < state; i++)
      wait += waitChar;

    if (state == 3)
      state = 1;
    else
      state++;

    std::cout << "Waiting" << wait << std::endl;

    service.sleep(1);
  }

  this->play();
}

void Game::searchOpponent() {
  Socket &socket = Socket::getInstance();

  std::string username = Auth::user["username"];
  socket.send("searchOpponent", username);

  this->waiting();
}

void Game::mate() {
  Socket &socket = Socket::getInstance();

  std::string username = Auth::user["username"];
  socket.send("lose", username);

  Game::isNeedToFinishAGame = true;
}

void Game::profile() {
  auto screen = ScreenInteractive::Fullscreen();

  auto component = Renderer([&] {
    return center(vcenter(vbox(
               {text("Profile") | bold | ftxui::color(Color::Yellow) | center,
                filler() | size(HEIGHT, EQUAL, 1),
                text("ID       : " +
                     std::to_string(Auth::user["id"].get<int>())),
                text("Email    : " + Auth::user["email"].get<std::string>()),
                text("Username : " + Auth::user["username"].get<std::string>()),
                filler() | size(HEIGHT, EQUAL, 1),
                text("Press Backspace to go back") | dim | center}))) |
           bgcolor(Color::Black);
  });

  auto finalComponent = CatchEvent(component, [&](Event event) {
    if (event == Event::Backspace) {
      screen.Exit();
      return true;
    }
    return false;
  });

  screen.Loop(finalComponent);
  this->menu();
}
