#ifndef HTTP_H
#define HTTP_H

#include <string>

using namespace std;

class Http {
public:
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
  string post(const string &endpoint, const string &data);
};

#endif
