#include "../include/program.h"
#include "../include/auth.h"

using namespace std;

Auth *auth = new Auth();

void Program::render() {
  bool isAuth = auth->refresh();

  if (!isAuth)
    auth->render();
}
