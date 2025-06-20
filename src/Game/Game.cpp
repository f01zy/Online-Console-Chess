#include "Game.h"
#include "../Auth/Auth.h"
#include "../Board/Board.h"
#include "../Figures/Figures.h"
#include "../Network/Http.h"
#include "../Network/Socket.h"
#include "../Utils/Utils.h"
#include "../globals.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <cstdlib>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace ftxui;

std::string Game::opponent = "";
std::string Game::color = "";
bool Game::isYourMove = false;
bool Game::isNeedToFinishAGame = false;

void Game::mode() {
  Utils utils;
  Http http;

  utils.clear();

  short choice;

  while (1) {
    std::vector<std::string> options = {"Online", "Local"};
    short choice = utils.menu(options, "Game mode");

    if (choice == 0) {
      setApiUrl(PRODUCTION_URL);
      break;
    }

    if (choice == 1) {
      setApiUrl(DEVELOPMENT_URL);
      break;
    }
  }

  std::string res = http.request("/status");

  if (res.empty()) {
    std::cout << "API is not available." << std::endl;
    exit(0);
  }
}

void Game::menu() {
  Utils utils;
  Auth auth;
  Socket &socket = Socket::getInstance();

  utils.clear();

  std::vector<std::string> options = {"Online game", "Profile", "Log out",
                                      "Exit", "Github"};
  short choice = utils.menu(options, "Menu");

  switch (choice) {
  case 0:
    isNeedToFinishAGame = false;
    isYourMove = false;
    opponent.clear();
    color.clear();
    searchOpponent();
    break;

  case 1:
    profile();
    break;

  case 2:
    auth.logout();
    break;

  case 3:
    exit(0);

  case 4:
    utils.openPageInBrowser(GITHUB);
    break;
  }

  menu();
}

void Game::start() {
  Auth auth;
  Game game;

  std::string isAuth = auth.refresh();

  if (isAuth.size() > 0)
    auth.render();

  else
    menu();
}

void Game::play() {
  Board &board = Board::getInstance();
  Utils utils;

  board.clear();
  utils.renderWithClear();

  while (!Game::isNeedToFinishAGame) {
    if (isYourMove)
      move();

    utils.sleep(0.01);
  }
}

void Game::move() {
  Socket &socket = Socket::getInstance();
  Board &board = Board::getInstance();
  Figures figures;
  Utils utils;

  std::string username = Auth::user["username"];
  std::string coordinates;

  InputOption inputOption;
  inputOption.multiline = false;

  Component inputCoordinates = Input(&coordinates, "Coordinates", inputOption);
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Container::Vertical({inputCoordinates});

  auto handleEnter = CatchEvent(component, [&](Event event) {
    if (event == Event::Return) {
      screen.Exit();
      return true;
    }
    return false;
  });

  auto renderer = Renderer(handleEnter, [&] {
    return center(vbox({
        filler() | size(HEIGHT, EQUAL, 2),
        inputCoordinates->Render() | size(WIDTH, EQUAL, 30),
    }));
  });

  screen.Loop(renderer);

  if (!figures.validateMove(figures.getCoordinates(coordinates))) {
    utils.clear();
    board.render("Don't valid coordinates.");
    return;
  }

  board.move(coordinates);
  Game::isYourMove = false;

  std::string data = username + " " + coordinates;
  socket.send("move", data);

  utils.renderWithClear();
}

void Game::waiting() {
  Utils utils;

  short state = 1;
  char waitChar = '.';

  while (opponent.size() == 0) {
    utils.clear();
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

    utils.sleep(1);
  }

  play();
}

void Game::searchOpponent() {
  Socket &socket = Socket::getInstance();

  std::string username = Auth::user["username"];
  socket.send("searchOpponent", username);

  waiting();
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
               {text("Profile") | bold | ftxui::color(Color::Yellow),
                filler() | size(HEIGHT, EQUAL, 1),
                text("ID       : " +
                     std::to_string(Auth::user["id"].get<int>())),
                text("Email    : " + Auth::user["email"].get<std::string>()),
                text("Username : " + Auth::user["username"].get<std::string>()),
                filler() | size(HEIGHT, EQUAL, 1),
                text("Press Backspace to go back") | dim}))) |
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
}
