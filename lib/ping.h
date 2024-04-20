#ifndef PING_INCLUDE
#define PING_INCLUDE

#include "config.h"
#include <ESP32Ping.h>

class Pinger {
  public: 
  static unsigned int last;

  static void init() {
    const int length = sizeof(LED_PINS) / sizeof(short); 
    for(int i=0; i<length ; i++) {
      pinMode(LED_PINS[i], OUTPUT);
      digitalWrite(LED_PINS[i], LOW);
    }
  }

  static void update(int interval=-1) {
    if (millis() - last < interval && interval > 0) return;
    last = millis();
    
    for (int i=0 ; i<DEVICE_COUNT ; i++) {
      devices[i].status = ping_start(devices[i].ip, 1, 1, 32, 100);
      if (i >= sizeof(LED_PINS) / sizeof(short)) continue;
      digitalWrite(LED_PINS[i], devices[i].status);
    }
  }
};

unsigned int Pinger::last = millis();

#endif