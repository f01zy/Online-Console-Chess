#include "../include/service.h"

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

short Service::select(vector<string> options) {
  for (short i = 0; i < options.size(); i++) {
    cout << options[i] << " (" << i + 1 << ")" << endl;
  }

  cout << endl;
  short choice = this->getNumber("Your choice: ");

  if (choice > options.size() + 1)
    return -1;

  return choice;
}

void Service::clear() { system("clear"); }
void Service::sleep(short seconds) {
  this_thread::sleep_for(chrono::seconds(1));
}

string Service::charToString(char symbol) {
  string temp(1, symbol);
  return temp;
}

short Service::getAlphabetIndex(char letter) {
  letter = tolower(letter);

  if (letter >= 'a' && letter <= 'z')
    return letter - 'a';

  else
    return -1;
}

short Service::getNumber(string message) {
  short result;

  while (true) {
    cout << message;

    cin >> result;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    else {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      break;
    }
  }

  return result;
}

void Service::openPageInBrowser(string page) {
  string command;

#if defined(_WIN32) || defined(_WIN64)
  command = "start " + page;
#elif defined(__APPLE__) || defined(__MACH__)
  command = "open " + page;
#else
  command = "xdg-open " + page;
#endif

  system(command.c_str());
}
