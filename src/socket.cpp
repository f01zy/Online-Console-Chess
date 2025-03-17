#include "../include/socket.h"
#include "../include/auth.h"
#include "../include/board.h"
#include "../include/game.h"
#include "../include/service.h"

#include <cstdlib>
#include <functional>
#include <sio_message.h>
#include <sio_socket.h>
#include <string>
#include <vector>

using namespace std;
using namespace sio;

void findOpponent(sio::event &event) {
  auto data = event.get_message()->get_vector();
  auto first = data[0]->get_vector();
  auto second = data[1]->get_vector();

  string username = Auth::user["username"];
  short opponentIndex = -1;

  if (first[0]->get_string() == username) {
    opponentIndex = 1;
  }

  else if (second[0]->get_string() == username) {
    opponentIndex = 0;
  }

  if (opponentIndex != -1) {
    string color = data[opponentIndex]->get_vector()[1]->get_string();

    Game::opponent = data[opponentIndex]->get_vector()[0]->get_string();
    Game::color = color;

    if (color == "white")
      Game::isYourMove = true;
  }
}

void moveEvent(sio::event &event) {
  Board board;

  auto data = event.get_message()->get_vector();
  string username = data[0]->get_string();
  string move = data[1]->get_string();

  if (Game::opponent == username) {
    board.move(move);
    Game::isYourMove = true;
  }
}

void opponentDisconnect(sio::event &event) {
  Service service;

  string username = event.get_message()->get_string();

  if (username == Game::opponent)
    Game::isDisconnect = true;
}

Socket::Socket(string url) {
  Service service;

  this->on("findOpponent", findOpponent);
  this->on("moveReceive", moveEvent);
  this->on("opponentDisconnect", opponentDisconnect);

  service.sleep(1);
  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() { c.socket()->close(); }

void Socket::send(string event, string data) { c.socket()->emit(event, data); }

void Socket::on(string event, function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}
