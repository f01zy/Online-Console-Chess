#pragma once

#include <string>

class Http {
public:
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);

  std::string request(std::string endpoint, std::string data = "");
};
