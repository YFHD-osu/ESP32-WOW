#include <WiFi.h>
#include "WebPage.h"

#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
using namespace httpsserver; // here

#define WIFI_SSID "SSID"
#define WIFI_PSK  "password"

HTTPServer httpServer = HTTPServer();

void setup() {
  Serial.begin(9600);
  
  Serial.printf("[WiFi] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PSK); //连接到wifi
  
  while (WiFi.status() != WL_CONNECTED) //等待连接
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("[WiFi] Connect success.");
  Serial.print("[WiFi] Local IP: "); Serial.println(WiFi.localIP());
  Serial.print("[WiFi] Subnet Mask: "); Serial.println(WiFi.subnetMask());
  delay(1000);

  ResourceNode *nodeRoot = new ResourceNode("/", "GET", &handleRoot); //建立卡片

  httpServer.registerNode(nodeRoot); //告訴伺服器有這張卡

  httpServer.start();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void sus (int number1, int number2){

}

void handleRoot(HTTPRequest * req, HTTPResponse * res) {
  // Status code is 200 OK by default.
  // We want to deliver a simple HTML page, so we send a corresponding content type:
  res->setHeader("Content-Type", "text/html");

  // The response implements the Print interface, so you can use it just like
  // you would write to Serial etc.
  res->println(index_html);
}