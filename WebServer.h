#include <SSLCert.hpp>
#include <HTTPServer.hpp>
#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

#include "config.h"
#include "Oled.h"
#include "Ping.h"
#include "mbedtls.h"
#include "WebPages.h"
#include "WakeOnLan.h"
#include "logger.h"

using namespace httpsserver;

class SecureWebServer: public HTTPSServer {
  public:
    SecureWebServer() : HTTPSServer(&cert, 443) {};

  private:
    SSLCert cert = SSLCert(
      example_crt_DER, example_crt_DER_len,
      example_key_DER, example_key_DER_len
    );
};

class WebServer: public HTTPServer {
  public:
    WebServer(): HTTPServer(80) {};
    
    static char* key;

    void begin(void);
    static bool is_authenticated(HTTPRequest*);
    static void handleWake(HTTPRequest*, HTTPResponse*);
    static void handlePing(HTTPRequest*, HTTPResponse*);
    static void handle404(HTTPRequest* , HTTPResponse*);
    static void handleRoot(HTTPRequest* , HTTPResponse*);
    static void handleLogin(HTTPRequest* , HTTPResponse*);
    static void handleLogout(HTTPRequest* , HTTPResponse*);
    static void handleSession(HTTPRequest* , HTTPResponse*);

    ResourceNode *nodeLogin   = new ResourceNode("/login",  "GET",  &handleLogin);
    ResourceNode *nodeSession = new ResourceNode("/login",  "POST", &handleSession);
    ResourceNode *nodeLogout  = new ResourceNode("/logout", "GET",  &handleLogout);
    ResourceNode *nodeRoot    = new ResourceNode("/",       "GET",  &handleRoot);
    ResourceNode *wakeDevice  = new ResourceNode("/wake",   "POST", &handleWake);
    ResourceNode *pingDevice  = new ResourceNode("/refresh","GET",  &handlePing);
    ResourceNode *node404     = new ResourceNode("",        "GET",  &handle404);

};

char* WebServer::key = generateUID(); // Generate a random AES key

void WebServer::begin(void) { 
  WebServer::registerNode(nodeLogin);
  WebServer::registerNode(nodeLogout);
  WebServer::registerNode(nodeSession);
  WebServer::registerNode(nodeRoot);
  WebServer::registerNode(wakeDevice);
  WebServer::registerNode(pingDevice);
  WebServer::setDefaultNode(node404);
  
  LOGI("WebServer", "Starting server with AES key: %s", key);

  WebServer::start();
  if (WebServer::isRunning()) {
    LOGI("WebServer", "Server is ready");
    OLED.draw_SERVER(gImage_server_connected);
  } else {
    LOGE("WebServer", "Server starts failed, restarting EPS32");
    OLED.draw_SERVER(gImage_server_failure);
    delay(1000);
    ESP.restart();
  }
  return;
}

// 建立登入頁面 
void WebServer::handleLogin(HTTPRequest *req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html; charset=utf-8");
  res->println(login_html);
}

//處理登出請求
void WebServer::handleLogout(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("set-cookie", "ESPSESSION=0");
  res->setHeader("Location", "/login");
}

// 處理喚醒請求
void WebServer::handleWake(HTTPRequest *req, HTTPResponse * res) {
  if (!(is_authenticated(req))) return;
  
  ResourceParameters * params = req->getParams();
  std::string deviceID;
  params->getQueryParameter("device", deviceID);

  if (deviceID == "0") WOL.sendMagicPacket(MACAddress[0]); //喚醒裝置
  else if (deviceID == "1") WOL.sendMagicPacket(MACAddress[1]);
  else if (deviceID == "2") WOL.sendMagicPacket(MACAddress[2]);
  LOGD("WebServer", "Got wake requests (Device: %s)", deviceID.c_str());
  
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
void WebServer::handlePing(HTTPRequest * req, HTTPResponse * res) {
  if (is_authenticated(req)) {
    pinger.update(1000);
    res->setHeader("Content-Type", "text/plain; charset=utf-8");
    for (int i=0 ; i<3 ; i++)
      res->print(pinger.deviceStatus[i]);
    return;
  }

  LOGD("WebServer", "Unauthenticated Client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");  
}

// 建立根(控制喚醒)頁面
void WebServer::handleRoot(HTTPRequest * req, HTTPResponse * res) {
  if (is_authenticated(req)) {
    LOGD("WebServer", "Unauthenticated Client!");
    res->setStatusCode(200);
    res->setStatusText("OK");
    res->setHeader("Content-Type", "text/html; charset=utf-8");
    res->println(index_html);
    return;
  }
  LOGD("WebServer", "Unauthenticated Client!");
  req->discardRequestBody();
  res->setStatusCode(301);
  res->setStatusText("Moved Permanently");
  res->setHeader("Location", "/login");
}

//處理未註冊的頁面
void WebServer::handle404(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(404);
  res->setStatusText("Not Found");
  res->setHeader("Content-Type", "text/html");
  res->println(not_found_html);
}

//認證登入訊息
bool WebServer::is_authenticated(HTTPRequest * req){
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

void connectWifi(int timeout) { 
  int timeCounter = 0;
  
  LOGI("WiFi", "Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PSK); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    timeCounter ++;
    OLED.draw_WIFI(timeCounter%2 ? gImage_wifi_connecting : gImage_wifi_blank);
    if (timeCounter > timeout) break;
  }
  
  if (timeCounter > timeout) {
    LOGE("WiFi", "Connect timeout, trying to reconnect");
    OLED.draw_WIFI(gImage_wifi_failure);
    connectWifi(20);
    return;
  }

  LOGI("WiFi", "Connect success.\n Local IP:%s\n Subnet Mask: %s", WiFi.localIP().toString(), WiFi.subnetMask().toString());
  OLED.draw_WIFI(gImage_wifi_connected);

}