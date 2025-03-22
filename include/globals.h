#pragma once
#include <string>
#include <unordered_map>

using namespace std;

extern string API_URL;

string getApiUrl();
void setApiUrl(string url);

const string DEVELOPMENT_URL = "http://localhost:3000";
const string PRODUCTION_URL = "http://77.95.201.197:3000";

const string GITHUB = "https://github.com/f01zy/Online-Console-Chess";
