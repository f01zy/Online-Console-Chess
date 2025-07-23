#include "Auth.h"
#include "../Game/Game.h"
#include "../Network/Http.h"
#include "../Utils/Utils.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/detail/value_t.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace ftxui;
json Auth::user = json::object();

void Auth::render(std::string error) {
  Utils utils;
  Http http;
  Game game;

  while (1) {
    utils.clear();

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

    auto fixedInput = [](Component input) {
      return input->Render() | size(WIDTH, EQUAL, 30);
    };

    std::vector<std::string> options = {"Sing In", "Sign Up", "Exit"};
    short choice =
        utils.menu(options, error.size() == 0 ? "Authentication" : error);
    auto screen = ScreenInteractive::Fullscreen();
    utils.clear();

    if (choice == 0) {
      auto component = Container::Vertical({inputEmail, inputPassword});

      auto handleEnter = CatchEvent(component, [&](Event event) {
        if (event == Event::Return) {
          screen.Exit();
          return true;
        }
        return false;
      });

      auto renderer = Renderer(handleEnter, [&] {
        return center(vcenter(vbox({
                   text("Type Enter to confirm") | bold | color(Color::Yellow),
                   filler() | size(HEIGHT, EQUAL, 1),
                   hbox(text("Email    : "), fixedInput(inputEmail)),
                   hbox(text("Password : "), fixedInput(inputPassword)),
               }))) |
               bgcolor(Color::Black);
      });
      screen.Loop(renderer);

      std::string res = signIn(email, password);
      if (res.size() == 0) {
        game.menu();
        return;
      }

      else {
        error = res;
      }
    }

    else if (choice == 1) {
      auto component = Container::Vertical(
          {inputEmail, inputUsername, inputPassword, inputConfirm});

      auto handleEnter = CatchEvent(component, [&](Event event) {
        if (event == Event::Return) {
          screen.Exit();
          return true;
        }
        return false;
      });

      auto renderer = Renderer(handleEnter, [&] {
        return center(vcenter(vbox({
                   text("Type Enter to confirm") | bold | color(Color::Yellow),
                   filler() | size(HEIGHT, EQUAL, 1),
                   hbox(text("Email            : "), fixedInput(inputEmail)),
                   hbox(text("Username         : "), fixedInput(inputUsername)),
                   hbox(text("Password         : "), fixedInput(inputPassword)),
                   hbox(text("Confirm password : "), fixedInput(inputConfirm)),
               }))) |
               bgcolor(Color::Black);
      });
      screen.Loop(renderer);

      if (!validatePassword(password, confirm)) {
        error = "Passwords don't match.";
        continue;
      }

      std::string res = signUp(email, username, password);
      if (res.size() == 0) {
        game.menu();
        return;
      }

      else {
        error = res;
      }
    }

    else if (choice == 2) {
      exit(0);
    }
  }
}

bool Auth::validatePassword(std::string password,
                            std::string confirm_password) {
  return password == confirm_password;
}

std::string Auth::signUp(std::string email, std::string username,
                         std::string password) {
  Http http;
  std::string fields =
      "email=" + email + "&username=" + username + "&password=" + password;
  std::string res = http.request("/auth/register", fields);
  json data = json::parse(res);

  return successRequestCallback(res);
}

std::string Auth::signIn(std::string email, std::string password) {
  Http http;
  std::string fields = "email=" + email + "&password=" + password;
  std::string res = http.request("/auth/login", fields);
  json data = json::parse(res);

  return successRequestCallback(res);
}

void Auth::writeRefreshToken(std::string token) {
  std::ofstream tokenFile("token.txt");

  if (tokenFile) {
    tokenFile << token << std::endl;
  }
}

void Auth::successAuthCallback(json data) {
  writeRefreshToken(data["refreshToken"]);
  user = data["user"];
}

std::string Auth::refresh() {
  Http http;

  std::string refreshToken = this->getRefreshToken();
  std::string fields = "refreshToken=" + refreshToken;
  std::string res = http.request("/auth/refresh", fields);

  return successRequestCallback(res);
}

std::string Auth::successRequestCallback(std::string res) {
  try {
    json data = json::parse(res);

    if (data.contains("refreshToken")) {
      successAuthCallback(data);
      return "";
    }

    else {
      return data["message"];
    }
  }

  catch (const json::parse_error &e) {
    return "Response parse error.";
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

  std::string refreshToken = getRefreshToken();
  std::string fields = "refreshToken=" + refreshToken;
  http.request("/auth/logout", fields);

  remove("token.txt");
  render();
}
