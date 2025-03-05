#include "../include/game.h"
#include "../include/auth.h"
#include "../include/service.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Game::menu() {
  Service service;
  Auth auth;

  service.clear();

  vector<string> options = {"Online game", "Github", "Log out", "Exit"};

  for (short i = 0; i < options.size(); i++) {
    cout << options[i] << " (" << i + 1 << ")" << endl;
  }

  short choice;
  cin >> choice;

  switch (choice) {
  case 3:
    auth.logout();
    break;

  case 4:
    exit(0);

  default:
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
