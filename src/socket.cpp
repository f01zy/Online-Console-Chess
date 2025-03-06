#include "../include/socket.h"
#include "../include/game.h"
#include "../include/service.h"

#include <functional>
#include <sio_socket.h>

using namespace std;
using namespace sio;

void startGame(sio::event &event) {
  string data = event.get_message()->get_string();
  Game::opponent = data;
}

Socket::Socket(string url) {
  Service service;
  Game game;

  this->on("startGame", startGame);

  service.sleep(1);
  c.set_logs_quiet();
  c.connect(url);
}

Socket::~Socket() { c.socket()->close(); }

void Socket::send(string event, string data) { c.socket()->emit(event, data); }

void Socket::on(string event, function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}
