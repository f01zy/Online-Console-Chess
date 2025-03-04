#include "../include/program.h"
#include "../include/auth.h"
#include "../include/game.h"

using namespace std;

Auth auth;
Game game;

void Program::start() {
  bool isAuth = auth.refresh();

  if (!isAuth)
    auth.render();

  else
    game.menu();
}
