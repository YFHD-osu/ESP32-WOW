# ESP32 Wake on wan
Wake up computers with ESP32 through the WAN.

## Project introduction
Have you ever been outside and need to use the computer
at home but it is off? If so, then you need this project.
By using ESP32 to setup and HTTPS web server and handle WOL
request is power-saving.

## Webpage demostration
- Wake on lan gui: [link](https://yfhd-osu.github.io/ESP32-WOW/WebUI/index.html)
- Login page: [link](https://yfhd-osu.github.io/ESP32-WOW/WebUI/login.html)

## Libraries in this project
- [Mbed-TLS](https://github.com/Mbed-TLS/mbedtls) - AES encrypt library
- [EasyDDNS](https://github.com/ayushsharma82/EasyDDNS) - DUC address update library
- [u8g2](https://github.com/olikraus/u8g2) - OLED display driver
- [ESP32Ping](https://github.com/marian-craciunescu/ESP32Ping) (modified) - Ping device library
