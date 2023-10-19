#include "mbedtls/aes.h"
#include <WiFi.h>

#define WIFI_SSID "wifi"
#define WIFI_PSK  "pass"

#define USERNAME "admin"
#define PASSWORD "admin"

void setup() {
  Serial.begin(115200);
  while(!(connectWifi(20))) Serial.println(" -> Trying to reconnect."); 
  
  
  mbedtls_aes_context aes;
 
  char * key = "abcdefghijklmnop";
  char *input = "Tech tutorials x";
  unsigned char output[16];
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)input, output);
  mbedtls_aes_free( &aes );
 
  for (int i = 0; i < 16; i++) {
    char str[3];
    sprintf(str, "%02x", (int)output[i]);
    Serial.println(String(str));
  }
}
 
void loop() {
  IPAddress ip = WiFi.localIP();
  char dest[strlen(SITE_USER)+strlen(PASSWORD)+2+15];
  char ipBuffer[15];
  
  strcpy(dest, SITE_USER);
  strcat(dest, ":");
  strcat(dest, PASSWORD);
  strcat(dest, ":");
  sprintf(ipBuffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  strcat(dest, ipBuffer);
  
  
  Serial.println(dest);
  Serial.println(*dest);
  delay(1000);
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
