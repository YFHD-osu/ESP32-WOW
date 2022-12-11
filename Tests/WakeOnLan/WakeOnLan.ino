#include <WiFiUdp.h>
#include <WakeOnLan.h>
#include <WiFi.h>
#include "OLED.h"

#define wifiName  "maggie"     // WIFI 名稱
#define wifiPass  "047699530"  // WIFI 密碼

int timeCounter;
const char *MACAddress[] = {"00:00:00:00:00:00"};
const byte Buttons[] = {34,35,36,39};
const byte LED[] = {4,0,2,15};
bool buttonStates[] = {false, false, false, false};

WiFiUDP UDP;
WiFiServer server(80);
WakeOnLan WOL(UDP); // Pass WiFiUDP class

SSD1306 OLEDController;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) pinMode(Buttons[i], INPUT);
  for (int i = 0; i < 4; i++) pinMode(LED[i], OUTPUT);
  OLEDController.init();
  while(!(connectWifi(20))) Serial.println(" -> Trying to reconnect."); //不斷嘗試連線到WiFi直到成功
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());
  OLEDController.clearbuffer();
  OLEDController.sendbuffer();
  timeCounter = -1; repeatLED();
}

void loop() {
  
  for(int i=0; i<4 ; i++){
    if(digitalRead(Buttons[i]) == false && buttonStates[i] == false){
      delay(50);
      if(digitalRead(Buttons[i]) == false){
        WOL.sendMagicPacket(MACAddress[i]);
        buttonStates[i] = true;
      }
    }
    if (digitalRead(Buttons[i]) == true) buttonStates[i] = false;
  }
  
}

bool connectWifi(int timeout){
  timeCounter = 0;
  
  OLEDController.clearbuffer();
  OLEDController.println("嘗試連線至:");
  OLEDController.println(" WiFi: " + String(wifiName));
  OLEDController.println("");
  OLEDController.sendbuffer();
  
  Serial.printf("Connecting to WiFi: %s ", wifiName);
  WiFi.begin(wifiName, wifiPass); //连接到wifi
  
  while (WiFi.status() != WL_CONNECTED) //等待连接
  {
    delay(500);
    Serial.print(".");
    repeatLED();
    timeCounter ++;
    if (timeCounter > timeout) break;
  }
  
  if (timeCounter > timeout) {
    Serial.println(" -> Connect timeout.");
    OLEDController.println("->連線超時");
    OLEDController.sendbuffer();
    delay(1000);
    return false;
  }
  else {
    Serial.println(" -> Connect success.");
    Serial.print(" | Local IP: "); Serial.println(WiFi.localIP());
    Serial.print(" | Subnet Mask: "); Serial.println(WiFi.subnetMask());
    OLEDController.println("->連線成功");
    OLEDController.sendbuffer();
    delay(1000);
    return true;
  }
}

void repeatLED() {
  
  if (timeCounter == -1){
    for (int i=0; i<4 ; i++) digitalWrite(LED[i], LOW);
    return;
  }
  
  for (int i=0; i<4 ; i++){
    if (timeCounter % 4 == i) digitalWrite(LED[i], HIGH);
    else digitalWrite(LED[i], LOW);
  }
}
