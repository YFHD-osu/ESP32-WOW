#include "config.h"   // 導入設定
#include "easyddns.h" // DDNS更新
#include "Ping.h"     // Ping功能

#include "webPages.h" // 網頁原碼
#include "webServer.h"
#include "OLED.h"     // OLED狀態

#include <WiFi.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

unsigned long startTime;
WebServer webServer = WebServer();

void setup() {
  Serial.begin(115200); // Enable serial port logging
  OLED.u8g2::setI2CAddress(0x7A);
  OLED.begin();         // Enable OLED display
  connectWifi(20);      // Connect to wifi until success 
  ddns.begin();         // DDNS 更新
  pinger.init();        // 初始化Ping
  
  startTime = millis();
  xTaskCreatePinnedToCore(webserver_handle, "WebServer", 6144, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  
}

void loop() {
  ddns.update(10*1000);
  if (millis() - startTime > 60*60*1000) {
    ESP.restart();
  };
  delay(1);
}

void webserver_handle(void *params) {
  webServer.begin();    // Start webserver
  while(1) {
    webServer.loop();
    delay(1);
  }
}