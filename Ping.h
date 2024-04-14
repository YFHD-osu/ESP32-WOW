#ifndef PING_INCLUDE
#define PING_INCLUDE

#include "config.h"
#include <ESP32Ping.h>

const int pins[] = {4, 0, 2, 15};

class PingHandler {
  public:
    void init();
    void update(int interval=-1);
    
    
    bool deviceStatus[DEVICE_COUNT];
  private:
    unsigned int lastPing = millis();
};

void PingHandler::init() {
  for(int i=0; i<4 ; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
}

void PingHandler::update(int interval) {
  if (millis() - lastPing < interval && interval > 0) return;
  lastPing = millis();
  
  for (int i=0 ; i<DEVICE_COUNT ; i++) {
    deviceStatus[i] = ping_start(devices[i].ip, 1, 1, 32, 100);
    digitalWrite(pins[i], deviceStatus[i]);
  }

}
  
PingHandler pinger;

#endif