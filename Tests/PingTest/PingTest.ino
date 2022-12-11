#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

void waitForWiFiConnectOrReboot(bool printOnSerial=true) {
  uint32_t notConnectedCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      if(printOnSerial) {
        Serial.println("Wifi connecting...");
      }
      notConnectedCounter++;
      if(notConnectedCounter > 50) { // Reset board if not connected after 5s
          if(printOnSerial) {
            Serial.println("Resetting due to Wifi not connecting...");
          }
          ESP.restart();
      }
  }
  if(printOnSerial) {
    // Print wifi IP addess
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
#define LED_BUILTIN 2
void setup() {
  Serial.begin(115200);
  WiFi.begin("wifi", "pass");
  // Wait for wifi to be connected
  waitForWiFiConnectOrReboot();
  // Initialize LED
  pinMode(LED_BUILTIN,OUTPUT);

}
void loop() {
  Serial.println("[Start Ping]");
  if(ping_start(IPAddress(192, 168, 1, 119), 1, 1, 32, 100)) {
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println(" ms");
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Error :(");
  }
  Serial.println("[End Ping]");
}