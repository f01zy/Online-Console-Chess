#include <chrono>
#include <iostream>
#include <sio_client.h>
#include <thread>

using namespace std;
using namespace sio;

void pong(sio::event &) { cout << "[INFO] pong return" << endl; }

int main() {
  client h;

  h.set_open_listener([]() { cout << "connected to server." << endl; });
  h.socket()->on("pong", &pong);

  h.connect("http://localhost:3000");

  while (true) {
    cout << "[INFO] send requrest" << endl;
    h.socket()->emit("ping");
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  h.sync_close();

  return 0;

  return 0;
}
