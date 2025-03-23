#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Auth {
public:
  static json user;

  std::string getRefreshToken();
  bool refresh();
  void logout();
  void render(std::string error = "");
  bool successRequestCallback(std::string res);
  bool successAuthCallback(json data);
  void writeRefreshToken(std::string token);
  bool sign_up(std::string email, std::string username, std::string password);
  bool sign_in(std::string email, std::string password);
  bool validate_password(std::string password, std::string confirm_password);
};
