#include "Socket.h"
#include "../Auth/Auth.h"
#include "../Board/Board.h"
#include "../Game/Game.h"
#include "../Utils/Utils.h"
#include "../globals.h"
#include <functional>
#include <vector>

using namespace sio;

void findOpponent(sio::event &event) {
  auto data = event.get_message()->get_vector();
  auto first = data[0]->get_vector();
  auto second = data[1]->get_vector();

  std::string username = Auth::user["username"];
  short opponentIndex = -1;

  if (first[0]->get_string() == username) {
    opponentIndex = 1;
  }

  else if (second[0]->get_string() == username) {
    opponentIndex = 0;
  }

  if (opponentIndex != -1) {
    std::string color = data[opponentIndex]->get_vector()[1]->get_string();

    Game::opponent = data[opponentIndex]->get_vector()[0]->get_string();
    Game::color = color;

    if (color == "white") {
      Game::isYourMove = true;
    }
  }
}

void moveEvent(sio::event &event) {
  Board &board = Board::getInstance();

  auto data = event.get_message()->get_vector();
  std::string username = data[0]->get_string();
  std::string move = data[1]->get_string();

  if (Game::opponent == username) {
    board.move(move);
    Game::isYourMove = true;
  }
}

void opponentLose(sio::event &event) {
  std::string username = event.get_message()->get_string();

  if (username == Game::opponent) {
    Game::isNeedToFinishAGame = true;
  }
}

Socket::Socket(std::string url) {
  Utils utils;

  this->on("findOpponent", findOpponent);
  this->on("moveReceive", moveEvent);
  this->on("opponentLose", opponentLose);

  utils.sleep(1);
  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() {
  this->c.clear_con_listeners();
  this->c.sync_close();
}

void Socket::send(std::string event, std::string data) {
  c.socket()->emit(event, data);
}

void Socket::on(std::string event, std::function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}

Socket &Socket::getInstance() {
  static Socket instance(getApiUrl());
  return instance;
}
