#define WIFI_SSID "maggie"
#define WIFI_PSK  "047699530"

#define USERNAME "YFHD"
#define PASSWORD "Nmsl7414"

//DDNS Update
#include <EasyDDNS.h>

// Encryption
#include "mbedtls.h"

// Datas
#include "WebPages.h"

// Certificates
#include "certificates/cert.h"
#include "certificates/private_key.h"

// OLED
#include "OLED.h"

// ESP32-HTTPS-Server
#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
using namespace httpsserver;

// Generate encryption key
char* generateUID();
char* key = generateUID();

SSLCert cert = SSLCert(
  example_crt_DER, example_crt_DER_len,
  example_key_DER, example_key_DER_len
);

HTTPSServer secureServer = HTTPSServer(&cert, 444);
WiFiServer server(80);
SSD1306 OLEDController;

void handleLogin(HTTPRequest * req, HTTPResponse * res);
void handleSession(HTTPRequest * req, HTTPResponse * res);
void handleRoot(HTTPRequest * req, HTTPResponse * res);
void handle404(HTTPRequest * req, HTTPResponse * res);

void setup() {
  Serial.begin(115200);  // For serial logging
  OLEDController.begin();// For OLED logging

  // 不斷嘗試連線到WiFi直到成功
  OLEDController.beginInfo(2,3,3);
  while(!(connectWifi(20))) Serial.println(" -> Trying to reconnect."); 

  // DDNS 更新
  OLEDController.beginInfo(1,2,3);
  server.begin();
  EasyDDNS.service("noip");
  EasyDDNS.client("mchill.ddns.net", "yfhd8964", "eason941106");
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP) {
    Serial.print("[DDNS Update] IP Change Detected: ");
    Serial.println(newIP);
  });
  EasyDDNS.update(0);
  
  // 註冊頁面到伺服器 ☒☑→⧖
  OLEDController.beginInfo(1,1,2);
  ResourceNode *nodeLogin = new ResourceNode("/login", "GET", &handleLogin);
  ResourceNode *nodeSession = new ResourceNode("/session", "POST", &handleSession);
  ResourceNode *nodeRoot  = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode *node404   = new ResourceNode("", "GET", &handle404);
  secureServer.registerNode(nodeLogin);
  secureServer.registerNode(nodeSession);
  secureServer.registerNode(nodeRoot);
  secureServer.setDefaultNode(node404);
  
  // 啟動網站伺服器
  Serial.println("Starting HTTPS server...");
  secureServer.start();
  
  if (secureServer.isRunning()) {
    Serial.println("Server is ready.");
    OLEDController.beginInfo(1,1,1);
  }else{ OLEDController.beginInfo(1,1,0); }

  Serial.printf("Currently AES key: %s\n", key);
}

void loop() {
  secureServer.loop();
  EasyDDNS.update(60000); // Check for new public IP every 60 seconds
  delay(1);
}

// 建立登入頁面 
void handleLogin(HTTPRequest *req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html; charset=utf-8");
  res->println(login_html);
}

// 處理登入請求
void handleSession(HTTPRequest *req, HTTPResponse * res) {  
  char requestBody[9+strlen(USERNAME)+9+strlen(PASSWORD)+2];
  char expectedHeader[9+strlen(USERNAME)+9+strlen(PASSWORD)+2];
  char *pch;
    
  // 擷取請求的資料
  req->readChars(requestBody, 9+strlen(USERNAME)+9+strlen(PASSWORD)+1);
  // 計算預期的資料
  strcpy(expectedHeader, "username=");
  strcat(expectedHeader, USERNAME);
  strcat(expectedHeader, "&password=");
  strcat(expectedHeader, PASSWORD);

  /* For debug purpose...*/
  Serial.println("Client Post: ");
  Serial.printf("Request  Body: %s\n", requestBody);
  Serial.printf("Expected Body: %s\n", expectedHeader);
  if (strstr(expectedHeader, requestBody) != NULL){ // 比對預期的資料是否在請求中
    Serial.println("Pass!");
    
    IPAddress clientIP = req->getClientIP(); // 取得使用者IP
    char* combineResult = combineChar(clientIP); // 合成字串
    String cookieString = "ESPSESSION=" + encryptAES256(key, combineResult);
    int str_len = cookieString.length() + 1;
    char cookieChar[str_len];
    cookieString.toCharArray(cookieChar, str_len);

    req->discardRequestBody();
    res->setStatusCode(301);
    res->setStatusText("Moved Permanently");
    res->setHeader("set-cookie", cookieChar);
    res->setHeader("Location", "/?r=0");
  }else{
    Serial.println("Deny!");
  }
  
}

// 建立根(控制喚醒)頁面
void handleRoot(HTTPRequest * req, HTTPResponse * res) {
  if (is_authenticated(req)) {
    Serial.println("Client Authenticated!");
    res->setStatusCode(200);
    res->setStatusText("OK");
    res->setHeader("Content-Type", "text/html; charset=utf-8");
    res->println(index_html);
    return;
  }
  Serial.println("Unauthenticated Client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");
}

//認證登入訊息
bool is_authenticated(HTTPRequest * req){
  String headers = req->getHeader("Cookie").c_str();
  
  if (headers.indexOf("ESPSESSION=") == -1) return false;

  // 整陀丟進 AES-256 裡加密
  IPAddress clientIP = req->getClientIP(); // 取得使用者IP
  char* combineResult = combineChar(clientIP); // 合成字串
  String encryptResult = encryptAES256(key, combineResult);
  /* For debug purpose...
  Serial.printf("IP Addr: %d.%d.%d.%d", clientIP[0], clientIP[1], clientIP[2], clientIP[0]);
  Serial.print("Headers: "); Serial.println(headers.c_str());
  Serial.println("Session: " + encryptResult);
  */
  
  if(headers.indexOf(encryptResult) != -1) return true;
  else return false;
}

//處理未註冊的頁面
void handle404(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(404);
  res->setStatusText("Not Found");
  res->setHeader("Content-Type", "text/html");
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Not Found</title></head>");
  res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
  res->println("</html>");
}

bool connectWifi(int timeout){
  int timeCounter = 0;
  
  Serial.printf("Connecting to WiFi: %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PSK); //连接到wifi
  
  while (WiFi.status() != WL_CONNECTED) //等待连接
  {
    delay(500);
    Serial.print(".");
    timeCounter ++;
    if (timeCounter > timeout) break;
  }
  
  if (timeCounter > timeout) {
    Serial.println(" -> Connect timeout.");
    delay(1000);
    return false;
  }
  else {
    Serial.println(" -> Connect success.");
    Serial.print(" | Local IP: "); Serial.println(WiFi.localIP());
    Serial.print(" | Subnet Mask: "); Serial.println(WiFi.subnetMask());
    delay(1000);
    return true;
  }
}
