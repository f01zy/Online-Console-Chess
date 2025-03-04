#pragma once
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;

class Auth {
public:
  json user;

  bool successRequestCallback(string res);
  bool refresh();
  bool successAuthCallback(json data);
  void render(string error = "");
  void writeRefreshToken(const string &token);
  bool sign_up(const string &email, const string &username,
               const string &password);
  bool sign_in(const string &email, const string &password);
  bool validate_password(const string &password,
                         const string &confirm_password);
};
