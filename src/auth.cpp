#include "../include/auth.h"
#include "../include/http.h"
#include "../include/service.h"

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;

Service *service = new Service();
Http *http = new Http();

void Auth::render(string error) {
  do {
    service->clear();

    if (!error.empty()) {
      cout << error << endl;
    }

    int choice;
    string email;
    string username;
    string password;
    string confirm_password;

    cout << "============== Chess Online =============" << endl;
    cout << "===== Sign In (1) ===== Sign Up (2) =====" << endl;
    cin >> choice;

    if (choice == 1) {
      service->clear();

      cout << "Enter your email: ";
      cin >> email;

      cout << "Enter your password: ";
      cin >> password;

      if (sign_in(email, password)) {
        // Вызываем функцию рендера игры.
        return;
      }

      else {
        error = "Cannot Sign In";
      }
    }

    else if (choice == 2) {
      bool passwords_match = false;
      int attempts = 3;

      cout << "Change email: ";
      cin >> email;

      cout << "Change username: ";
      cin >> username;

      while (!passwords_match && attempts > 0) {
        cout << "Create password: ";
        cin >> password;

        cout << "Confirm password: ";
        cin >> confirm_password;

        if (validate_password(password, confirm_password)) {
          passwords_match = true;
          bool isAuth = sign_up(email, username, password);

          if (isAuth) {
            // Вызываем функцию рендера игры.
            return;
          }

          else {
            error = "Cannot Sign Up.";
          }
        }

        else {
          attempts--;
          error = "Passwords do not match! Remaining attempts: " +
                  to_string(attempts);

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

bool Auth::validate_password(const string &password,
                             const string &confirm_password) {
  return password == confirm_password;
}

bool Auth::sign_up(const string &email, const string &username,
                   const string &password) {
  string fields =
      "email=" + email + "&username=" + username + "&password=" + password;

  string res = http->post("/auth/register", fields);

  json data = json::parse(res);

  return this->successRequestCallback(res);
}

bool Auth::sign_in(const string &email, const string &password) {
  string fields = "email=" + email + "&password=" + password;

  string res = http->post("/auth/login", fields);

  json data = json::parse(res);

  return this->successRequestCallback(res);
}

void Auth::writeRefreshToken(const string &token) {
  ofstream tokenFile("token.txt");

  if (tokenFile) {
    tokenFile << token << endl;
  }
}

bool Auth::successAuthCallback(json data) {
  this->writeRefreshToken(data["refreshToken"]);
  this->user = data["user"];

  return true;
}

bool Auth::refresh() {
  string refreshToken;

  ifstream tokenFile("token.txt");

  if (tokenFile.is_open()) {
    getline(tokenFile, refreshToken);
    tokenFile.close();
  }

  string fields = "refreshToken=" + refreshToken;
  string res = http->post("/auth/refresh", fields);

  return this->successRequestCallback(res);
}

bool Auth::successRequestCallback(string res) {
  try {
    json data = json::parse(res);

    if (data.contains("refreshToken"))
      return this->successAuthCallback(data);

    return false;
  }

  catch (const json::parse_error &e) {
    cout << "JSON parse error: " << e.what() << endl;
    return false;
  }
}
