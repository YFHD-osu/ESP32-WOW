# ESP32 WakeOnWan
Wake up computers with ESP32 over Wide Area Network.

## Project Introduction
Have you ever been outside and need to use the computer
at home but it is off? If so, then you need this project.
Using ESP32 to setup a HTTPS web server and handle WOL
request is a power-saving method.

## Webpage Demostration
- Wake on lan gui: [link](https://yfhd-osu.github.io/ESP32-WOW/web/index.html)
- Login page: [link](https://yfhd-osu.github.io/ESP32-WOW/web/login.html)

## Setup
### 1. Installation Dependences
| Title | Description |
| :---- | :---------- |
| [u8g2](https://github.com/olikraus/u8g2) | OLED display driver |
| [Mbed-TLS](https://github.com/Mbed-TLS/mbedtls) | AES encrypt library |
| [EasyDDNS](https://github.com/ayushsharma82/EasyDDNS) | DUC address update library |
| [ESP32Ping](https://github.com/marian-craciunescu/ESP32Ping/releases/tag/1.6) | Ping device library |
| [ESP32HttpsSever](https://github.com/fhessel/esp32_https_server) | Web server library |

#### In ``ESP32Ping`` library, change line 292 to 293 in ``ping.cpp ``
```cpp
// Timeout
  tout.tv_sec = 0; // tout.tv_sec = timeout;
  tout.tv_usec = timeout*1000; //tout.tv_usec = 0;
```

#### In ``ESP32HttpsSever`` library, change line 8 in ``src\HTTPConnection.hpp``
```cpp
#include <mbedtls/base64.h>
#include <esp32/sha.h> //#include <hwcrypto/sha.h>
#include <functional>
```

### 2. Configure ESP32 Server Settings
- Clone this repository if you haven't.
- Rename ``config.h.example`` to ``config.h`` and copy it into  ``\lib`` folder
- Follow instructions in ``config.h`` to complete settings.

#### Device Section
``devices`` variable is an list of ``Device`` object, and every ``Device`` object should be initialized like below 
```cpp
Device(ID, IP-ADDRESS, MAC-ADDRESS, DEVICE-NAME, IMAGE-ASSETS, DESCRIPTION)
```
| Variable | Type |  Description                     |
| :------- | :--- | :------------------------------- |
| ID | ``int`` | Serial ID starts from 0 for each device |
| IP-ADDRESS | ``IPAddress`` | Local area network IPv4 address |
| MAC-ADDRESS | ``char[18]`` | Mac address string object |
| DEVICE-NAME | ``String`` | Device name shows on website |
| IMAGE-ASSETS | ``String`` | URL to the image as device icon on website |
| DESCRIPTION | ``String`` | Device description below title on website (Gray out text) |

#### WebServer Section


## Dynamic Security Authentication
#### 中文名稱: 
- 動態安全認證
#### 特色：
1. 與一般Session 加密不同，本加密協議可以避免 Session ID 洩漏時有心人士可從不同IP登入。
3. 由於微處理機不容易實現時效性Cookie 所以每次重啟微處理機時，會自動登出所有裝置。
4. 適合用於記憶體空間小到靠北的主機，例如ESP32

#### 技術核心：
1. 加密方式：AES-256 (美國聯邦政府採用的一種區塊加密標準)加密協議。
2. 原文格式：使用者名稱:使用者密碼:登入IP
3. AES-256金鑰：每次重啟時重新隨機生成
4. 不會將任何Session ID 存入記憶體，節省空間也保障安全

#### 運作流程：
1. 伺服器開啟時設定隨機的AES 金鑰
2. 主機連入時提取Client IP
3. 以”使用者名稱：密碼：IP” 格式合併字串
4. 及時使用AES 加密計算Session ID 並驗證

## 參考文獻
1. [輸入密碼錯誤時如何提示](https://stackoverflow.com/questions/6242976/javascript-hide-show-element)

2. [Safari 的 vh 有點陰間](https://ithelp.ithome.com.tw/m/articles/10249090)
