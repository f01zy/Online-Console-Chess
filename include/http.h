#pragma once
#include <string>

using namespace std;

class Http {
public:
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);

  string request(string endpoint, string data = "");
};
