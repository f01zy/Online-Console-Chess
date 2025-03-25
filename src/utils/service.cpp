#include "../../include/service.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <thread>
#include <vector>

using namespace ftxui;

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

short Service::menu(std::vector<std::string> options, std::string message) {
  auto screen = ScreenInteractive::Fullscreen();
  int selected = 0;

  auto menu = Menu(&options, &selected);

  auto component = CatchEvent(menu, [&](Event event) {
    if (event == Event::Return) {
      screen.Exit();
      return true;
    }
    return false;
  });

  auto renderer = Renderer(component, [&] {
    return center(vcenter(vbox(text(message) | bold | color(Color::Yellow),
                               filler() | size(HEIGHT, EQUAL, 1),
                               component->Render()))) |
           bgcolor(Color::Black);
  });

  screen.Loop(renderer);
  return selected;
}
