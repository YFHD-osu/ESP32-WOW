#include <SSLCert.hpp>
#include <HTTPServer.hpp>
#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

#include "web.h"
#include "wol.h"
#include "oled.h"
#include "ping.h"
#include "config.h"
#include "logger.h"
#include "mbedtls.h"

using namespace httpsserver;

class SecureWebServer: public HTTPSServer {
  public:
    SecureWebServer() : HTTPSServer(&cert, 443) {};

  private:
    SSLCert cert = SSLCert(
      crt_DER, crt_DER_len,
      key_DER, key_DER_len
    );
};

class WebServer: public HTTPServer {
  public:
    WebServer(): HTTPServer(80) {};
    
    static char* key;

    void begin(void);
    static bool is_authenticated(HTTPRequest*);
    static void handleList(HTTPRequest* , HTTPResponse*);
    static void handleWake(HTTPRequest*, HTTPResponse*);
    // static void handlePing(HTTPRequest*, HTTPResponse*); (Departured)
    static void handle404(HTTPRequest* , HTTPResponse*);
    static void handleRoot(HTTPRequest* , HTTPResponse*);
    static void handleLogin(HTTPRequest* , HTTPResponse*);
    static void handleLogout(HTTPRequest* , HTTPResponse*);
    static void handleSession(HTTPRequest* , HTTPResponse*);

    ResourceNode *pingList  = new ResourceNode("/list", "GET",  &handleList);
    ResourceNode *nodeLogin   = new ResourceNode("/login",  "GET",  &handleLogin);
    ResourceNode *nodeSession = new ResourceNode("/login",  "POST", &handleSession);
    ResourceNode *nodeLogout  = new ResourceNode("/logout", "GET",  &handleLogout);
    ResourceNode *nodeRoot    = new ResourceNode("/",       "GET",  &handleRoot);
    ResourceNode *wakeDevice  = new ResourceNode("/wake",   "POST", &handleWake);
    // ResourceNode *pingDevice  = new ResourceNode("/refresh","GET",  &handlePing);
    ResourceNode *node404     = new ResourceNode("",        "GET",  &handle404);

};

char* WebServer::key = generateUID(); // Generate a random AES key

void WebServer::begin(void) { 
  WebServer::registerNode(nodeLogin);
  WebServer::registerNode(nodeLogout);
  WebServer::registerNode(nodeSession);
  WebServer::registerNode(nodeRoot);
  WebServer::registerNode(wakeDevice);
  WebServer::registerNode(pingList);
  // WebServer::registerNode(pingDevice);
  WebServer::setDefaultNode(node404);
  
  LOGI("WebServer", "Starting server with AES key: %s", key);

  WebServer::start();
  if (WebServer::isRunning()) {
    LOGI("WebServer", "Server is ready");
    OLED.drawServer(gImage_server_connected);
  } else {
    LOGE("WebServer", "Server starts failed, restarting EPS32");
    OLED.drawServer(gImage_server_failure);
    delay(1000);
    ESP.restart();
  }
  return;
}

// Establish login page
void WebServer::handleLogin(HTTPRequest *req, HTTPResponse * res) {
  res->setHeader("Expires", "0");
  res->setHeader("Pragma", "no-cache");
  res->setHeader("Cache-Control", "no-cache, must-revalidate");

  if (is_authenticated(req)) {
    res->setStatusCode(301);
    res->setHeader("Location", "/");
    res->setStatusText("Moved Permanently");
    return;
  }

  res->setHeader("Content-Type", "text/html; charset=utf-8");
  res->println(login_html);
}

void WebServer::handleLogout(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Expires", "0");
  res->setHeader("Pragma", "no-cache");
  res->setHeader("Cache-Control", "no-cache, must-revalidate");
  
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("set-cookie", "ESPSESSION=0");
  res->setHeader("Location", "/login");
}

void WebServer::handleWake(HTTPRequest *req, HTTPResponse * res) {
  if (!(is_authenticated(req))) {
    res->setStatusCode(403);
    return;
  };
  
  ResourceParameters * params = req->getParams();
  std::string deviceID;
  params->getQueryParameter("device", deviceID);

  WOL.sendMagicPacket(devices[std::stoi(deviceID)].mac); // Wake device
  LOGD("WebServer", "Got wake requests (Device: %s)", deviceID.c_str());
  res->setStatusCode(200);
  req->discardRequestBody();
}

// 處理登入請求
void WebServer::handleSession(HTTPRequest *req, HTTPResponse * res) {  
  char requestBody[9+strlen(SITE_USER)+9+strlen(SITE_PASS)+5] = "";
  char expectedHeader[9+strlen(SITE_USER)+9+strlen(SITE_PASS)+3];

  // 擷取請求的資料
  req->readChars(requestBody, 9+strlen(SITE_USER)+9+strlen(SITE_PASS)+2);
  // 計算預期的資料
  strcpy(expectedHeader, "username=");
  strcat(expectedHeader, SITE_USER);
  strcat(expectedHeader, "&password=");
  strcat(expectedHeader, SITE_PASS);
  strcat(expectedHeader, "&");

  /* For debug purpose...*/
  LOGD("WebServer", "Client Post:\n Request  Body: %s\n Expected Body: %s\n", requestBody, expectedHeader);
  if (strstr(expectedHeader, requestBody) != NULL){ // 比對預期的資料是否在請求中
    LOGD("WebServer", "Authenticate pass!");
    
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
    res->setHeader("Location", "/");
  }else{
    req->discardRequestBody();
    res->setStatusCode(301);
    res->setStatusText("Moved Permanently");
    res->setHeader("set-cookie", "ESPSESSION=0");
    res->setHeader("Location", "/login");
  }
}

// 處理更新裝置 (Departured)
/*void WebServer::handlePing(HTTPRequest * req, HTTPResponse * res) {
  if (is_authenticated(req)) {
    res->setHeader("Content-Type", "text/plain; charset=utf-8");
    res->setHeader("Cache-Control", "no-store, no-cache, must-revalidate");
    for (int i=0 ; i<DEVICE_COUNT ; i++)
      res->print(devices[i].status);
    return;
  }

  LOGD("WebServer", "Unauthenticated Client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");  
}*/

void WebServer::handleList(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Expires", "0");
  res->setHeader("Pragma", "no-cache");
  res->setHeader("Cache-Control", "no-store, no-cache, must-revalidate");

  if (is_authenticated(req)) {
    LOGI("ListDev", "OK LISTING %d devices", DEVICE_COUNT);
    res->setHeader("Content-Type", "application/json; charset=UTF-8");
    res->print("[");
    for (int i=0 ; i<DEVICE_COUNT ; i++) {
      res->print(devices[i].toJson());
      if (i+1 != DEVICE_COUNT) res->print(",");
    }
    res->print("]");
    return;
  }
  
  LOGD("WebServer", "Unauthenticated client!");
  req->discardRequestBody();
  res->setStatusCode(403);
  res->setStatusText("Forbidden");
  res->setHeader("Content-Type", "text/html");
  res->println(forbidden_html);
  return;
}

// 建立根(控制喚醒)頁面
void WebServer::handleRoot(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Expires", "0");
  res->setHeader("Pragma", "no-cache");
  res->setHeader("Cache-Control", "no-cache, must-revalidate");

  if (is_authenticated(req)) {
    res->setStatusCode(200);
    res->setStatusText("OK");
    res->setHeader("Content-Type", "text/html; charset=utf-8");
    res->println(index_html);
    return;
  }

  LOGD("WebServer", "Unauthenticated client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");
}

// 處理未註冊的頁面
void WebServer::handle404(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(404);
  res->setStatusText("Not Found");
  res->setHeader("Content-Type", "text/html");
  res->println(not_found_html);
  return;
}

// Validate login secret
bool WebServer::is_authenticated(HTTPRequest * req){
  String headers = req->getHeader("Cookie").c_str();
  
  if (headers.indexOf("ESPSESSION=") == -1) return false;

  // AES-256 encryption
  IPAddress clientIP = req->getClientIP(); // Get client IP
  char* combineResult = combineChar(clientIP); // Merge string
  String encryptResult = encryptAES256(key, combineResult);
  
  LOGD("AUTH", "IP Addr: %d.%d.%d.%d", clientIP[0], clientIP[1], clientIP[2], clientIP[0]);
  LOGD("AUTH", "Headers: %s", headers.c_str());
  LOGD("AUTH", "Session: %s", encryptResult.c_str());
  
  if(headers.indexOf(encryptResult) != -1) return true;
  else return false;
}

void connectWifi(int timeout) { 
  int timeCounter = 0;
  
  LOGI("WiFi", "Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PSK); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    timeCounter ++;
    OLED.drawWIFI(timeCounter%2 ? gImage_wifi_connecting : gImage_wifi_blank);
    if (timeCounter > timeout) break;
  }
  
  if (timeCounter > timeout) {
    LOGE("WiFi", "Connect timeout, trying to reconnect");
    OLED.drawWIFI(gImage_wifi_failure);
    connectWifi(20);
    return;
  }

  LOGI("WiFi", "Connect success.\n Local IP:%s\n Subnet Mask: %s", WiFi.localIP().toString(), WiFi.subnetMask().toString());
  OLED.drawWIFI(gImage_wifi_connected);

}