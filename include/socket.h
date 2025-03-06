#pragma once

#include <memory>
#include <sio_client.h>
#include <sio_socket.h>
#include <string>

using namespace std;
using namespace sio;

class Socket {
public:
  static Socket &getInstance(string url) {
    static Socket instance(url);
    return instance;
  }

  void send(string event, string data);
  void on(string event, function<void(sio::event &)> callback);

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

private:
  Socket(string url);
  ~Socket();

  client c;
};
