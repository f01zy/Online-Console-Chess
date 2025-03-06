#include "../include/socket.h"
#include <functional>
#include <sio_socket.h>

using namespace std;
using namespace sio;

Socket::Socket(string url) { c.connect(url); }

Socket::~Socket() { c.socket()->close(); }

void Socket::send(string event, string data) { c.socket()->emit(event, data); }

void Socket::on(string event, function<void(sio::event &)> callback) {
  c.socket()->on(event, callback);
}
