#include "Cipher.h"

Cipher * cipher = new Cipher();

char * key = "abcdefghijklmnop";
String data = "ESP32 AES128bit Encryption example";
String cipherString, decipheredString;

void setup() {
  Serial.begin(115200);
  Serial.println("Data In: " + data);
}

void loop() {
  key = generateUID();
  cipher->setKey(key);
  Serial.printf("Current Key: %s \n", key);
  
  cipherString = cipher->encryptString(data);
  Serial.println("Encrypt String: " + cipherString);
  
  decipheredString = cipher->decryptString(cipherString);
  Serial.println("Decrypted String: " + decipheredString);
  
  delay(1000);
}

char * generateUID(){
  /* Change to allowable characters */
  const char possible[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  static char uid[16 + 1];
  for(int p = 0, i = 0; i < 16; i++){
    int r = random(0, strlen(possible));
    uid[p++] = possible[r];
  }
  uid[16] = '\0';
  return uid;
}
