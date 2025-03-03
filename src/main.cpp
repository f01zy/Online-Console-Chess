#include <fstream>
#include <string>

#include "../include/auth.h"

using namespace std;

Auth *auth = new Auth();

int main() {
  ifstream tokenFile("token.txt");
  string token;

  getline(tokenFile, token);

  if (token.empty()) {
    auth->render();
  }

  return 0;
}
