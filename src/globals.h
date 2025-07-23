#pragma once

#include <string>

extern std::string API_URL;
std::string getApiUrl();
void setApiUrl(std::string url);

const std::string PRODUCTION_URL = "http://localhost:3000";
const std::string DEVELOPMENT_URL = "http://localhost:3000";
const std::string GITHUB = "https://github.com/f01zy/Chess";
