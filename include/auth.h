#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;

class Auth {
public:
  static json user;

  string getRefreshToken();
  bool refresh();
  void logout();
  void render(string error = "");
  bool successRequestCallback(string res);
  bool successAuthCallback(json data);
  void writeRefreshToken(string token);
  bool sign_up(string email, string username, string password);
  bool sign_in(string email, string password);
  bool validate_password(string password, string confirm_password);
};
