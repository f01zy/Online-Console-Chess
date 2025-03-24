#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <variant>

using json = nlohmann::json;

class Auth {
public:
  static json user;

  std::string refresh();
  void logout();
  void render(std::string error = "");
  std::string sign_up(std::string email, std::string username,
                      std::string password);
  std::string sign_in(std::string email, std::string password);

  bool validatePassword(std::string password, std::string confirm_password);
  std::string successRequestCallback(std::string res);
  void successAuthCallback(json data);
  void writeRefreshToken(std::string token);
  std::string getRefreshToken();
};
