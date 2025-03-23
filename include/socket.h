#pragma once

#include "../include/globals.h"

#include <memory>
#include <sio_client.h>
#include <sio_socket.h>
#include <string>

using namespace sio;

class Socket {
public:
  static Socket &getInstance() {
    static Socket instance(getApiUrl());
    return instance;
  }

  void send(std::string event, std::string data);
  void on(std::string event, std::function<void(sio::event &)> callback);

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

private:
  Socket(std::string url);
  ~Socket();

  client c;
};
