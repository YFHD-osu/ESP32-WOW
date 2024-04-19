#ifndef PING_INCLUDE
#define PING_INCLUDE

#include "config.h"
#include <ESP32Ping.h>

class Pinger {
  public: 

  static unsigned int last;
  static int pins[4];

  static void init() {
    for(int i=0; i<4 ; i++) {
      pinMode(pins[i], OUTPUT);
      digitalWrite(pins[i], LOW);
    }
  }

  static void update(int interval=-1) {
    if (millis() - last < interval && interval > 0) return;
    last = millis();
    
    for (int i=0 ; i<DEVICE_COUNT ; i++) {
      devices[i].status = ping_start(devices[i].ip, 1, 1, 32, 100);
      digitalWrite(pins[i], devices[i].status);
    }
  }
};

int Pinger::pins[4] = {4, 0, 2, 15};
unsigned int Pinger::last = millis();

#endif