#if OLED_DISPLAY

#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

class SSD1306 {
  public:
    void begin();
    void println(String context);
    void sendBuffer();
    void clearBuffer();
    void beginInfo(int wifi, int ddns, int webServer);
    
  private:
    int yPos = 14;
};

void SSD1306::begin() {
  u8g2.begin();
  u8g2.enableUTF8Print();  // 啟動 UTF8 支援
  return;
}

void SSD1306::println (String context) {
  u8g2.setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2.setCursor(0, yPos); //自動換行
  u8g2.print(context);
  yPos += 16;
  return;
}

void SSD1306::sendBuffer () {  //顯示暫存器內容
  u8g2.sendBuffer();
  return;
}

void SSD1306::clearBuffer () { //清除暫存器內容
  yPos = 14;
  u8g2.clearBuffer();
  return;
}

void SSD1306::beginInfo(int wifi, int ddns, int webServer){ // 更新開機時的OLED顯示內容
  u8g2.clearBuffer();
  yPos = 14;
  u8g2.setFont(u8g2_font_unifont_t_chinese1); // 使用 chinese1字型檔
  u8g2.setCursor(0, yPos); //自動換行
  u8g2.print("啟動進度:");

  yPos += 16;
  u8g2.setCursor(0, yPos); //自動換行
  switch(wifi){
    case 0:
      u8g2.print("☒WiFi連線");
      break;
    case 1:
      u8g2.print("☑WiFi連線");
      break;
    case 2:
      u8g2.print("→WiFi連線");
      break;
    case 3:
      u8g2.print("⧖WiFi連線");
  }

  yPos += 16;
  u8g2.setCursor(0, yPos); //自動換行
  switch(ddns){
    case 0:
      u8g2.print("☒DNS更新");
      break;
    case 1:
      u8g2.print("☑DNS更新");
      break;
    case 2:
      u8g2.print("→DNS更新");
      break;
    case 3:
      u8g2.print("⧖DNS更新");
  }

  yPos += 16;
  u8g2.setCursor(0, yPos); //自動換行
  switch(webServer){
    case 0:
      u8g2.print("☒網站伺服器");
      break;
    case 1:
      u8g2.print("☑網站伺服器");
      break;
    case 2:
      u8g2.print("→網站伺服器");
      break;
    case 3:
      u8g2.print("⧖網站伺服器");
  }
  
  u8g2.sendBuffer();
}

#else

class SSD1306 {
  public:
    void begin();
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

SSD1306 OLEDController;