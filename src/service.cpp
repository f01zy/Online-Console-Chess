#include "../include/service.h"

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std;

void Service::clear() { system("clear"); }
void Service::sleep(short seconds) {
  this_thread::sleep_for(chrono::seconds(1));
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
