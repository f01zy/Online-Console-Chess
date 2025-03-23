#include "../include/auth.h"
#include "../include/game.h"
#include "../include/http.h"
#include "../include/service.h"

#include <cstdio>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

json Auth::user = json::object();

void Auth::render(std::string error) {
  Service service;
  Http http;
  Game game;

  do {
    service.clear();

    if (!error.empty()) {
      std::cout << error << std::endl;
      std::cout << std::endl;
    }

    std::string email;
    std::string username;
    std::string password;
    std::string confirm_password;

    std::vector<std::string> options = {"Sing In", "Sign Up"};
    short choice = service.select(options);

    service.clear();
    if (choice == 1) {
      std::cout << "Enter your email: ";
      std::cin >> email;

      std::cout << "Enter your password: ";
      std::cin >> password;

      if (sign_in(email, password)) {
        game.menu();
        return;
      }

      else {
        error = "Cannot Sign In";
      }
    }

    else if (choice == 2) {
      bool passwords_match = false;
      short attempts = 3;

      std::cout << "Change email: ";
      std::cin >> email;

      std::cout << "Change username: ";
      std::cin >> username;

      while (!passwords_match && attempts > 0) {
        std::cout << "Create password: ";
        std::cin >> password;

        std::cout << "Confirm password: ";
        std::cin >> confirm_password;

        if (validate_password(password, confirm_password)) {
          passwords_match = true;
          bool isAuth = sign_up(email, username, password);

          if (isAuth) {
            game.menu();
            return;
          }

          else {
            error = "Cannot Sign Up.";
          }
        }

        else {
          attempts--;
          error = "Passwords do not match! Remaining attempts: " +
                  std::to_string(attempts);

          if (attempts <= 0) {
            error = "Too many attempts. Please try again.";
          }
        }
      }
    }

    else
      error = "Invalid choice!";
  } while (!error.empty());

  curl_global_cleanup();
}

bool Auth::validate_password(std::string password,
                             std::string confirm_password) {
  return password == confirm_password;
}

bool Auth::sign_up(std::string email, std::string username,
                   std::string password) {
  Http http;
  std::string fields =
      "email=" + email + "&username=" + username + "&password=" + password;

  std::string res = http.request("/auth/register", fields);

  json data = json::parse(res);

  return this->successRequestCallback(res);
}

bool Auth::sign_in(std::string email, std::string password) {
  Http http;
  std::string fields = "email=" + email + "&password=" + password;

  std::string res = http.request("/auth/login", fields);

  json data = json::parse(res);

  return this->successRequestCallback(res);
}

void Auth::writeRefreshToken(std::string token) {
  std::ofstream tokenFile("token.txt");

  if (tokenFile) {
    tokenFile << token << std::endl;
  }
}

bool Auth::successAuthCallback(json data) {
  this->writeRefreshToken(data["refreshToken"]);
  this->user = data["user"];

  return true;
}

bool Auth::refresh() {
  Http http;
  std::string refreshToken = this->getRefreshToken();

  std::string fields = "refreshToken=" + refreshToken;
  std::string res = http.request("/auth/refresh", fields);

  return this->successRequestCallback(res);
}

bool Auth::successRequestCallback(std::string res) {
  try {
    json data = json::parse(res);

    if (data.contains("refreshToken"))
      return this->successAuthCallback(data);

    return false;
  }

  catch (const json::parse_error &e) {
    std::cout << "JSON parse error: " << e.what() << std::endl;
    return false;
  }
}

std::string Auth::getRefreshToken() {
  std::string refreshToken;

  std::ifstream tokenFile("token.txt");

  if (tokenFile.is_open()) {
    getline(tokenFile, refreshToken);
    tokenFile.close();
  }

  return refreshToken;
}

void Auth::logout() {
  Http http;
  std::string refreshToken = this->getRefreshToken();

  std::string fields = "refreshToken=" + refreshToken;
  http.request("/auth/logout", fields);

  remove("token.txt");

  this->render();
}
