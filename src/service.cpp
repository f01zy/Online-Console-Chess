#include "../include/service.h"

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

short Service::select(std::vector<std::string> options) {
  for (short i = 0; i < options.size(); i++) {
    std::cout << options[i] << " (" << i + 1 << ")" << std::endl;
  }

  std::cout << std::endl;
  short choice = this->getNumber("Your choice: ");

  if (choice > options.size() + 1)
    return -1;

  return choice;
}

void Service::clear() { system("clear"); }
void Service::sleep(short seconds) {
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

std::string Service::charToString(char symbol) {
  std::string temp(1, symbol);
  return temp;
}

short Service::getAlphabetIndex(char letter) {
  letter = tolower(letter);

  if (letter >= 'a' && letter <= 'z')
    return letter - 'a';

  else
    return -1;
}

short Service::getNumber(std::string message) {
  short result;

  while (true) {
    std::cout << message;

    std::cin >> result;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }

  return result;
}

void Service::openPageInBrowser(std::string page) {
  std::string command;

#if defined(_WIN32) || defined(_WIN64)
  command = "start " + page;
#elif defined(__APPLE__) || defined(__MACH__)
  command = "open " + page;
#else
  command = "xdg-open " + page;
#endif

  system(command.c_str());
}
