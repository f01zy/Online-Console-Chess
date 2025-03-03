#include "../include/auth.h"
#include "../include/http.h"
#include "../include/service.h"

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

Service *service = new Service();
Http *http = new Http();

void Auth::render() {
  service->clear();

  setlocale(LC_ALL, "EN_US.UTF-8");

  int choice;
  string email;
  string username;
  string password;
  string confirm_password;

  cout << "============== Chess Online =============" << endl;
  cout << "===== Sign In (1) ===== Sign Up (2) =====" << endl;
  cin >> choice;

  service->clear();
  switch (choice) {
  case 1:
    cout << "Enter your email: ";
    cin >> email;

    cout << "Enter your password: ";
    cin >> password;

    sign_in(email, password);

    break;

  case 2: {
    bool passwords_match = false;
    int attempts = 3;

    cout << "Change email: ";
    cin >> email;

    cout << "Change username: ";
    cin >> username;

    do {
      cout << "Create password: ";
      cin >> password;

      cout << "Confirm password: ";
      cin >> confirm_password;

      if (validate_password(password, confirm_password)) {
        passwords_match = true;
        sign_up(email, username, password);
      }

      else {
        service->clear();

        attempts--;
        cout << "Passwords do not match! Remaining attempts: ";
        cout << attempts << endl;

        if (attempts <= 0) {
          service->clear();
          this->render();
        }
      }
    } while (!passwords_match && attempts > 0);

    break;
  }

  default:
    cout << "Invalid choice!" << endl;
    break;
  }

  curl_global_cleanup();
}

bool Auth::validate_password(const string &password,
                             const string &confirm_password) {
  return password == confirm_password;
}

void Auth::sign_up(const string &email, const string &username,
                   const string &password) {
  string fields =
      "email=" + email + "&username=" + username + "&password=" + password;

  string res = http->post("/auth/register", fields);

  json data = json::parse(res);
  string refreshToken;

  if (data.contains("refreshToken")) {
    this->writeRefreshToken(data["refreshToken"]);
  }
}

void Auth::sign_in(const string &email, const string &password) {
  string fields = "email=" + email + "&password=" + password;

  string res = http->post("/auth/login", fields);

  json data = json::parse(res);

  if (data.contains("refreshToken")) {
    this->writeRefreshToken(data["refreshToken"]);
  }
}

void Auth::writeRefreshToken(const string &token) {
  ofstream tokenFile("token.txt");

  if (tokenFile) {
    tokenFile << token << endl;
  }
}
