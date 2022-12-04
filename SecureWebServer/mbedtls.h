#include "mbedtls/aes.h"

String encryptAES256 (char* key, char* input) {
  //char * key = "abcdefghijklmnop";
  //char *input = "Tech tutorials x";
  unsigned char output[16];
  String result = "";
  
  mbedtls_aes_context aes;
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)input, output);
  mbedtls_aes_free( &aes );
 
  for (int i = 0; i < 16; i++) {
    char str[3];
    sprintf(str, "%02x", (int)output[i]);
    result += String(str);
    //Serial.print(output[i]);
  }
  return result;
}

// 生成隨機金鑰
char* generateUID(){
  const char possible[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  static char uid[16 + 1];
  for(int p = 0, i = 0; i < 16; i++){
    int r = random(0, strlen(possible));
    uid[p++] = possible[r];
  }
  uid[16] = '\0';
  return uid;
}

// 生成未加密的字串
char* combineChar(IPAddress clientIP) {
  char sessionSource[strlen(USERNAME)+strlen(PASSWORD)+2+15];
  char ipBuffer[15];

  return "asdasdasd";
  // 把 使用者 + 密碼 + 使用者IP 合成一個字串
  strcpy(sessionSource, USERNAME);
  strcat(sessionSource, ":");
  strcat(sessionSource, PASSWORD);
  strcat(sessionSource, ":");
  sprintf(ipBuffer, "%d.%d.%d.%d", clientIP[0], clientIP[1], clientIP[2], clientIP[3]); // 將IP轉為char
  strcat(sessionSource, ipBuffer);

  return sessionSource;
}
