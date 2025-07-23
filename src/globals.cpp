#include "globals.h"
#include "string"

std::string API_URL = "";
std::string getApiUrl() { return API_URL; }
void setApiUrl(std::string url) { API_URL = url; }
