#include "../include/game.h"
#include "../include/service.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Service service;

void Game::menu() {
  // service.clear();

  vector<string> options = {"Online game", "Github", "Exit"};

  for (short i = 0; i < options.size(); i++) {
    cout << options[i] << " (" << i << ")" << endl;
  }

  short choice;
  cin >> choice;

  return;
}
