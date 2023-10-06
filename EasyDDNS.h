#if DUC_SERVICE //檢查 DDNS 更新是否在config.h裡開啟 

#include <EasyDDNS.h>
#include <WiFi.h>
#include "HTTPClient.h"

WiFiServer server(80); // 為EasyDDNS開啟WiFi伺服器

class DDNSHandler {
  public:
    void init();
    void update();        
};

void DDNSHandler::init(){
  server.begin();

  EasyDDNS.service(domainType);
  EasyDDNS.client(domainName, domainUser, domainPass);
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP) 
    {
      Serial.print("[DDNS Update] IP Change Detected: ");
      Serial.println(newIP);
    }
  );
}

void DDNSHandler::update(){
  EasyDDNS.update(0);
}

DDNSHandler ddnsHandler;

#endif
