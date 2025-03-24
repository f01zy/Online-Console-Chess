#include "../../include/auth.h"
#include "../../include/game.h"
#include "../../include/http.h"
#include "../../include/service.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace ftxui;

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
    std::string confirm;

    InputOption inputOption;
    inputOption.multiline = false;

    Component inputEmail = Input(&email, "Email", inputOption);
    Component inputUsername = Input(&username, "Username", inputOption);

    inputOption.password = true;

    Component inputPassword = Input(&password, "Password", inputOption);
    Component inputConfirm = Input(&confirm, "Confirm password", inputOption);

    const int input_width = 30;

    auto fixedInput = [input_width](Component input) {
      return input->Render() | size(WIDTH, EQUAL, input_width);
    };

    std::vector<std::string> options = {"Sing In", "Sign Up"};
    short choice = service.select(options);

    service.clear();
    if (choice == 1) {
      auto component = Container::Vertical({inputEmail, inputPassword});

      auto renderer = Renderer(component, [&] {
        return center(vcenter(vbox({
                   text("Type Ctrl+C to confirm") | color(Color::Yellow),
                   filler() | size(HEIGHT, EQUAL, 1),
                   hbox(text("Email    : "), fixedInput(inputEmail)),
                   hbox(text("Password : "), fixedInput(inputPassword)),
               }))) |
               bgcolor(Color::Black);
      });

      auto screen = ScreenInteractive::Fullscreen();
      screen.Loop(renderer);

      std::string res = sign_in(email, password);

      if (res.size() == 0) {
        game.menu();
        return;
      }

      else
        error = res;
    }

    else if (choice == 2) {
      auto component = Container::Vertical(
          {inputEmail, inputUsername, inputPassword, inputConfirm});

      auto renderer = Renderer(component, [&] {
        return center(vcenter(vbox({
                   text("Type Ctrl+C to confirm") | color(Color::Yellow),
                   filler() | size(HEIGHT, EQUAL, 1),
                   hbox(text("Email            : "), fixedInput(inputEmail)),
                   hbox(text("Username         : "), fixedInput(inputUsername)),
                   hbox(text("Password         : "), fixedInput(inputPassword)),
                   hbox(text("Confirm password : "), fixedInput(inputConfirm)),
               }))) |
               bgcolor(Color::Black);
      });

      auto screen = ScreenInteractive::Fullscreen();
      screen.Loop(renderer);

      if (!validatePassword(password, confirm)) {
        error = "Passwords do not match";
        continue;
      }

      std::string res = sign_up(email, username, password);

      if (res.size() == 0) {
        game.menu();
        return;
      }

      else
        error = res;
    }
  } while (1);
}

bool Auth::validatePassword(std::string password,
                            std::string confirm_password) {
  return password == confirm_password;
}

std::string Auth::sign_up(std::string email, std::string username,
                          std::string password) {
  Http http;
  std::string fields =
      "email=" + email + "&username=" + username + "&password=" + password;
  std::string res = http.request("/auth/register", fields);
  json data = json::parse(res);

  return this->successRequestCallback(res);
}

std::string Auth::sign_in(std::string email, std::string password) {
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

void Auth::successAuthCallback(json data) {
  this->writeRefreshToken(data["refreshToken"]);
  this->user = data["user"];
}

std::string Auth::refresh() {
  Http http;

  std::string refreshToken = this->getRefreshToken();
  std::string fields = "refreshToken=" + refreshToken;
  std::string res = http.request("/auth/refresh", fields);

  return this->successRequestCallback(res);
}

std::string Auth::successRequestCallback(std::string res) {
  try {
    json data = json::parse(res);

    if (data.contains("refreshToken")) {
      this->successAuthCallback(data);
      return "";
    }

    else
      return data["message"];
  }

  catch (const json::parse_error &e) {
    return "Response parse error";
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
