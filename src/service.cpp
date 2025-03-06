#include "../include/service.h"

#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;

void Service::clear() { system("clear"); }
void Service::sleep(short seconds) {
  this_thread::sleep_for(chrono::seconds(1));
}
