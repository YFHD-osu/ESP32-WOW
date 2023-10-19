#ifndef PING_INCLUDE
#define PING_INCLUDE

#include "config.h"
#include <ESP32Ping.h>

class PingHandler {
  public:
    void init();
    void update(int interval=-1);
    
    bool deviceStatus[DEVICE_COUNT];
  private:
    unsigned int lastPing = millis();
};

void PingHandler::init() {
  update();
}

void PingHandler::update(int interval) {
  if (millis() - lastPing < interval && interval > 0) return;
  lastPing = millis();
  
  for (int i=0 ; i<DEVICE_COUNT ; i++)
    deviceStatus[i] = ping_start(ip[i], 1, 1, 32, 100);

}
  
PingHandler pinger;

#endif