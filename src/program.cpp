#include "../include/program.h"
#include "../include/auth.h"
#include "../include/game.h"

using namespace std;

void Program::start() {
  Auth auth;
  Game game;

  bool isAuth = auth.refresh();

  if (!isAuth)
    auth.render();

  else
    game.menu();
}
