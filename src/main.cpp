#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;

const string API_URL = "http://localhost:3000/api/v1";

bool validate_password(const string &password, const string &confirm_password) {
  return password == confirm_password;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

void sign_up(const string &email, const string &username,
             const string &password) {
  CURL *curl;
  CURLcode res;
  string readBuffer;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();

  if (curl) {
    string url = API_URL + "/auth/register";
    string fields =
        "email=" + email + "&username=" + username + "&password=" + password;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(
        headers, "Content-Type: application/x-www-form-urlencoded ");

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    } else {
      std::cout << readBuffer << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
}

bool sign_in(const string &username, const string &password) { return false; }

void clear() { system("clear"); }

void auth() {
  clear();

  setlocale(LC_ALL, "EN_US.UTF-8");

  int choice;
  string email;
  string username;
  string password;
  string confirm_password;

  cout << "        ===== Chess Online =====        " << endl;
  cout << "===== Sign In (1) ===== Sign Up (2) =====" << endl;
  cin >> choice;

  clear();
  switch (choice) {
  case 1:
    cout << "Enter your email: ";
    cin >> email;

    cout << "Enter your password: ";
    cin >> password;

    if (sign_in(email, password)) {
      cout << "Sign in case result" << endl;
    }

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
        clear();

        attempts--;
        cout << "Passwords do not match! Remaining attempts: ";
        cout << attempts << endl;

        if (attempts <= 0) {
          clear();
          auth();
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

int main() {
  ifstream tokenFile("token.txt");
  string token;

  getline(tokenFile, token);

  if (token.empty()) {
    auth();
  }

  return 0;
}
