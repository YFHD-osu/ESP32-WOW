#include "WiFi.h"
#include "HTTPClient.h"

#include <EasyDDNS.h>

const char * ssid = "wifi";
const char * password = "wifiPass";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP()); // Print the IP address
  server.begin();

  /*
    List of supported DDNS providers:
    - "duckdns"
    - "noip"
    - "dyndns"
    - "dynu"
    - "enom"
    - "all-inkl"
    - "selfhost.de"
    - "dyndns.it"
    - "strato"
    - "freemyip"
    - "afraid.org"
  */
  EasyDDNS.service("noip");

  /*
    For DDNS Providers where you get a token:
      Use this: EasyDDNS.client("domain", "token");

    For DDNS Providers where you get username and password: ( Leave the password field empty "" if not required )
      Use this: EasyDDNS.client("domain", "username", "password");
  */
  EasyDDNS.client("domain", "username", "password");

  // Get Notified when your IP changes
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP) {
    Serial.print("EasyDDNS - IP Change Detected: ");
    Serial.println(newIP);
  });
  EasyDDNS.update(0);
}

void loop() {
  // Check for new public IP every 10 seconds
  EasyDDNS.update(10000);
  Serial.println("asdasdasd");
  delay(1000);
}
