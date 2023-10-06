#include "config.h"   // 導入設定
#include "EasyDDNS.h" // DDNS更新
#include "Ping.h"     // Ping功能
#include "mbedtls.h"  // 登入加密
#include "WebPages.h" // 網頁原碼
#include "OLED.h"     // OLED狀態

// ESP32-HTTPS-Server
#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <WiFiUdp.h>
#include <WakeOnLan.h>

using namespace httpsserver;

// Random AES key
char* generateUID();
char* key = generateUID();

SSLCert cert = SSLCert(
  example_crt_DER, example_crt_DER_len,
  example_key_DER, example_key_DER_len
);

WiFiUDP UDP;
WakeOnLan WOL(UDP); 
HTTPSServer secureServer = HTTPSServer(&cert, 444);

void handleLogin(HTTPRequest * req, HTTPResponse * res);
void handleLogout(HTTPRequest * req, HTTPResponse * res);
void handleSession(HTTPRequest * req, HTTPResponse * res);
void handleRoot(HTTPRequest * req, HTTPResponse * res);
void handle404(HTTPRequest * req, HTTPResponse * res);

void setup() {
  Serial.begin(115200);  // For Serial logging
  OLEDController.begin();// For OLED logging

  // 不斷嘗試連線到WiFi直到成功
  OLEDController.beginInfo(2,3,3);
  while(!(connectWifi(20))) Serial.println("[WiFi] Trying to reconnect."); 

  // 初始化Ping
  pingHandler.init();

  // DDNS 更新
  #if DUC_SERVICE
    ddnsHandler.init();
    ddnsHandler.update(); // 更新網域名稱@EasyDDNS.h
    OLEDController.beginInfo(1,2,3);
  #else
    OLEDController.beginInfo(1,0,3);
  #endif

  // 註冊頁面到伺服器 ☒☑→⧖
  OLEDController.beginInfo(1,1,2);
  ResourceNode *nodeLogin   = new ResourceNode("/login",  "GET",  &handleLogin);
  ResourceNode *nodeSession = new ResourceNode("/login",  "POST", &handleSession);
  ResourceNode *nodeLogout  = new ResourceNode("/logout", "GET",  &handleLogout);
  ResourceNode *nodeRoot    = new ResourceNode("/",       "GET",  &handleRoot);
  ResourceNode *wakeDevice  = new ResourceNode("/wake",   "POST", &handleWake);
  ResourceNode *pingDevice  = new ResourceNode("/refresh","GET",  &handlePing);
  ResourceNode *node404     = new ResourceNode("",        "GET",  &handle404);

  secureServer.registerNode(nodeLogin);
  secureServer.registerNode(nodeLogout);
  secureServer.registerNode(nodeSession);
  secureServer.registerNode(nodeRoot);
  secureServer.registerNode(wakeDevice);
  secureServer.registerNode(pingDevice);
  secureServer.setDefaultNode(node404);
  
  // 啟動網站伺服器
  Serial.println("[WebServer] Starting HTTPS server...");
  secureServer.start();
  
  if (secureServer.isRunning()) {
    Serial.println("[WebServer] Server is ready.");
    OLEDController.beginInfo(1,1,1);
  }else{ OLEDController.beginInfo(1,1,0); }

  Serial.printf("[WebServer] Currently AES key: %s\n", key);
}

void loop() {
  secureServer.loop();
  delay(1);
}

// 建立登入頁面 
void handleLogin(HTTPRequest *req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html; charset=utf-8");
  res->println(login_html);
}

//處理登出請求
void handleLogout(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("set-cookie", "ESPSESSION=0");
  res->setHeader("Location", "/login");
}

// 處理喚醒請求
void handleWake(HTTPRequest *req, HTTPResponse * res) {
  if (!(is_authenticated(req))) return;
  
  ResourceParameters * params = req->getParams();
  std::string deviceID;
  params->getQueryParameter("device", deviceID);

  if (deviceID == "0") WOL.sendMagicPacket(MACAddress[0]); //喚醒裝置
  else if (deviceID == "1") WOL.sendMagicPacket(MACAddress[1]);
  else if (deviceID == "2") WOL.sendMagicPacket(MACAddress[2]);
  Serial.println(deviceID.c_str());
  
  req->discardRequestBody();
}

// 處理登入請求
void handleSession(HTTPRequest *req, HTTPResponse * res) {  
  char requestBody[9+strlen(USERNAME)+9+strlen(PASSWORD)+5] = "";
  char expectedHeader[9+strlen(USERNAME)+9+strlen(PASSWORD)+3];
  char *pch;
    
  // 擷取請求的資料
  req->readChars(requestBody, 9+strlen(USERNAME)+9+strlen(PASSWORD)+2);
  // 計算預期的資料
  strcpy(expectedHeader, "username=");
  strcat(expectedHeader, USERNAME);
  strcat(expectedHeader, "&password=");
  strcat(expectedHeader, PASSWORD);
  strcat(expectedHeader, "&");

  /* For debug purpose...*/
  Serial.println("[Debug] Client Post: ");
  Serial.printf("[Debug] Request  Body: %s\n", requestBody);
  Serial.printf("[Debug] Expected Body: %s\n", expectedHeader);
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
    req->discardRequestBody();
    res->setStatusCode(301);
    res->setStatusText("Moved Permanently");
    res->setHeader("set-cookie", "ESPSESSION=0");
    res->setHeader("Location", "/login?e=1");
  }
}

//處理更新裝置
void handlePing(HTTPRequest * req, HTTPResponse * res) {
  if (is_authenticated(req)) {
    pingHandler.update();
    res->setHeader("Content-Type", "text/plain; charset=utf-8");
    for (int i=0 ; i<3 ; i++)
      res->print(pingHandler.deviceStatus[i]);
    return;
  }
  Serial.println("[Debug] Unauthenticated Client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");  
}

// 建立根(控制喚醒)頁面
void handleRoot(HTTPRequest * req, HTTPResponse * res) {
  if (is_authenticated(req)) {
    Serial.println("[Debug] Client Authenticated!");
    res->setStatusCode(200);
    res->setStatusText("OK");
    res->setHeader("Content-Type", "text/html; charset=utf-8");
    res->println(index_html);
    return;
  }
  Serial.println("[Debug] Unauthenticated Client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");
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

// 連線至WiFi
bool connectWifi(int timeout){
  int timeCounter = 0;
  
  Serial.printf("[WiFi] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PSK); //连接到wifi
  
  while (WiFi.status() != WL_CONNECTED) //等待连接
  {
    delay(500);
    Serial.print(".");
    timeCounter ++;
    if (timeCounter > timeout) break;
  }
  
  if (timeCounter > timeout) {
    Serial.println("[WiFi] Connect timeout.");
    delay(1000);
    return false;
  }
  else {
    Serial.println("[WiFi] Connect success.");
    Serial.print("[WiFi] Local IP: "); Serial.println(WiFi.localIP());
    Serial.print("[WiFi] Subnet Mask: "); Serial.println(WiFi.subnetMask());
    delay(1000);
    return true;
  }
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
