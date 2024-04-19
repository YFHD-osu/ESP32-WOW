#ifndef EASY_DDNS_INCLUDE
#define EASY_DDNS_INCLUDE

#include <WiFi.h>
#include <EasyDDNS.h>

#include "oled.h"
#include "icons.h"
#include "config.h"
#include "logger.h"
// #include "HTTPClient.h"

class DDNSHandler : public EasyDDNSClass {
  public:
    DDNSHandler() : EasyDDNSClass() {};

    void begin(void);
    void update(int interval=-1);

  private:
    // Starts a wifi server for EasyDDNS
    WiFiServer server = WiFiServer(81);
    unsigned long lastUpdate = millis();
};

#if DUC_SERVICE
void DDNSHandler::begin(void) {
  server.begin();
  EasyDDNSClass::service(DOMAIN_TYPE);
  EasyDDNSClass::client(DOMAIN_NAME, DOMAIN_USER, DOMAIN_PASS);
  EasyDDNSClass::onUpdate([&](const char* oldIP, const char* newIP) 
    { LOGI("DDNS", "IP Change Detected: %s", newIP); }
  );
  update();
}

void DDNSHandler::update(int interval) {
  const unsigned int now = millis();
  if (now - lastUpdate < interval && interval > 0) return;
  lastUpdate = now;
  OLED.drawDDNS(gImage_DDNS_connecting);
  EasyDDNSClass::update(0);
  LOGI("DUC", "Peroidic IP updated.");
  OLED.drawDDNS(gImage_DDNS_connected);
}
#else
void DDNSHandler::begin(void) {}
void DDNSHandler::update(int interval) {}
#endif

DDNSHandler ddns;
#endif