#include <WiFi.h>

#define ASYNC_TCP_SSL_ENABLED true
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "WebPages.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "WiFi Name";
const char* password = "WiFi Pass";

const char* http_username = "admin";
const char* http_password = "admin";

const char* PARAM_INPUT_1 = "state";

const int output = 2;

AsyncWebServer httpServer(443);

int fileRequest(void * arg, const char *filename, uint8_t **buf) {
  Serial.printf("Reading file: %s\n", filename);
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS has started");
  }
  File file = SPIFFS.open(filename, "r");
  if(file){
    Serial.printf("File found: %s\n", filename);
    size_t size = file.size();
    uint8_t * nbuf = (uint8_t*)malloc(size);
    if(nbuf){
      size = file.read(nbuf, size);
      file.close();
      *buf = nbuf;
      return size;
    }
    file.close();
  } else {
    Serial.println("File not found!");
    *buf = 0;
    return 0;
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP()); // Print ESP Local IP Address

  // Route for root / web page
  httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!(is_authenticated(request))) //檢查是否登入
      request->redirect("/login");
    request->send_P(200, "text/html; charset=UTF-8", index_html);
  });

  httpServer.on("/wake", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!(is_authenticated(request))) //檢查是否登入
      request->send_P(403, "text/plain;", "403 Forbidden");
    request->send_P(200, "text/html; charset=UTF-8", index_html);
  });

  httpServer.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
    if (is_authenticated(request)) //檢查是否登入
      request->redirect("/");
    request->send_P(200, "text/html; charset=UTF-8", login_html);
  });
  
  httpServer.onNotFound([](AsyncWebServerRequest *request){
    request->send_P(404, "text/plain;", "404 Not Found");
  });

  httpServer.onSslFileRequest(fileRequest, NULL);
  
  // Start server
  httpServer.begin();
}
  
void loop() {
  
}

bool is_authenticated(AsyncWebServerRequest *request) {
  Serial.println("Enter is_authenticated");
  if (request->hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = request->header("Cookie");
    Serial.println(cookie);

    String token = (String(http_username) + ":" + String(http_password) + ":" + request->client()->remoteIP().toString()); //  token = sha1(token);

    if (cookie.indexOf("ESPSESSIONID=" + token) != -1) {
      Serial.println("Authentication Successful");
      return true;
    }
  }
  Serial.println("Authentication Failed");
  return false;
}

void logout(AsyncWebServerRequest *request) {
  Serial.println("Disconnection");
  
  AsyncWebServerResponse *response = request->beginResponse(301, "text/html");
  response->addHeader("Location", "/login.html?msg=User disconnected");
  response->addHeader("Cache-Control", "no-cache");
  response->addHeader("Set-Cookie", "ESPSESSIONID=0");
  
  request->send(response);

  return;
}
