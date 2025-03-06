#include "../include/socket.h"
#include "../include/auth.h"
#include "../include/game.h"
#include "../include/service.h"

#include <functional>
#include <sio_socket.h>
#include <string>

using namespace std;
using namespace sio;

void startGame(sio::event &event) {
  auto data = event.get_message()->get_vector();
  string first = data[0]->get_string();
  string second = data[1]->get_string();
  string username = Auth::user["username"];

  if (first == username)
    Game::opponent = second;

  else if (second == username)
    Game::opponent = first;
}

Socket::Socket(string url) {
  Service service;

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
