#pragma once

#include "../globals.h"
#include <memory>
#include <sio_client.h>
#include <string>

class Socket {
public:
  Socket(std::string url);
  Socket(Socket &) = delete;
  ~Socket();

  static Socket &getInstance() {
    static Socket instance(getApiUrl());
    return instance;
  }

  void send(std::string event, std::string data);
  void on(std::string event, std::function<void(sio::event &)> callback);

private:
  sio::client c;
};
