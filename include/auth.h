#pragma once
#include <string>

using namespace std;

class Auth {
public:
  void render();
  void writeRefreshToken(const string &token);
  void sign_up(const string &email, const string &username,
               const string &password);
  void sign_in(const string &email, const string &password);
  bool validate_password(const string &password,
                         const string &confirm_password);
};
