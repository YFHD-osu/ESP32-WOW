#ifndef OLED_INCLUDED
#define OLED_INCLUDED
#if OLED_DISPLAY

#include <Wire.h>
#include <U8g2lib.h>
#include "icons.h"

class u8g2: public U8G2_SSD1306_128X64_NONAME_F_HW_I2C {
  public: 
    u8g2(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) :
      U8G2_SSD1306_128X64_NONAME_F_HW_I2C(rotation, reset, clock, data) {};
};

class SSD1306: public u8g2  {
  public:
    SSD1306() : u8g2(U8G2_R0, U8X8_PIN_NONE) {};

    void begin();
    void println(String context);
    void sendBuffer();
    void clearBuffer();
    void beginInfo(int wifi, int ddns, int webServer);
    void draw_WIFI(const unsigned char*);
    void draw_DDNS(const unsigned char*);
    void draw_SERVER(const unsigned char*);

  private:
    int yPos = 14;
};

void SSD1306::begin() {
  u8g2::begin();
  u8g2::enableUTF8Print();  // 啟動 UTF8 支援
  draw_WIFI(gImage_wifi_blank);
  draw_SERVER(gImage_server_blank);
  draw_DDNS(DUC_SERVICE ? gImage_DDNS_balnk : gImage_DDNS_disabled);
  u8g2::sendBuffer();
  return;
}

void SSD1306::println(String context) {
  u8g2::setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2::setCursor(0, yPos); //自動換行
  u8g2::print(context);
  yPos += 16;
  return;
}

void SSD1306::clearBuffer () { //清除暫存器內容
  yPos = 14;
  u8g2::clearBuffer();
  return;
}

void SSD1306::draw_WIFI(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(112, 0, 16, 16, image);
  u8g2::sendBuffer();
}

void SSD1306::draw_SERVER(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(96, 0, 16, 16, image);
  u8g2::sendBuffer();
}

void SSD1306::draw_DDNS(const unsigned char* image) {
  u8g2::setFontMode(0);
  u8g2::setDrawColor(1);
  u8g2::drawXBM(80, 0, 16, 16, image);
  u8g2::sendBuffer();
}

void SSD1306::beginInfo(int wifi, int ddns, int webServer){ // 更新開機時的OLED顯示內容
  u8g2::clearBuffer();
  yPos = 14;
  u8g2::setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2::setCursor(0, yPos); //自動換行
  u8g2::print("啟動進度:");

  yPos += 16;
  u8g2::setCursor(0, yPos); //自動換行
  switch(wifi){
    case 0:
      u8g2::print("☒WiFi連線");
      break;
    case 1:
      u8g2::print("☑WiFi連線");
      break;
    case 2:
      u8g2::print("→WiFi連線");
      break;
    case 3:
      u8g2::print("⧖WiFi連線");
  }

  yPos += 16;
  u8g2::setCursor(0, yPos); //自動換行
  switch(ddns){
    case 0:
      u8g2::print("☒DNS更新");
      break;
    case 1:
      u8g2::print("☑DNS更新");
      break;
    case 2:
      u8g2::print("→DNS更新");
      break;
    case 3:
      u8g2::print("⧖DNS更新");
  }

  yPos += 16;
  u8g2::setCursor(0, yPos); //自動換行
  switch(webServer){
    case 0:
      u8g2::print("☒網站伺服器");
      break;
    case 1:
      u8g2::print("☑網站伺服器");
      break;
    case 2:
      u8g2::print("→網站伺服器");
      break;
    case 3:
      u8g2::print("⧖網站伺服器");
  }
  
  u8g2::sendBuffer();
}

#else

class SSD1306 {
  public:
    void begin(void);
    void println(String);
    void sendBuffer();
    void clearBuffer();
    void beginInfo(int, int, int);
};

void SSD1306::begin () {}
void SSD1306::println (String context) {}
void SSD1306::sendBuffer () {}
void SSD1306::clearBuffer () {}
void SSD1306::beginInfo (int wifi, int ddns, int webServer) {}

#endif

SSD1306 OLED;
#endif
