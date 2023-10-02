#include <ESP32Ping.h>

class PingHandler {
  public:
    bool deviceStatus[DEVICE_COUNT];
    void init();
    void update();
  private:
    unsigned int lastPing;
};

void PingHandler::init(){
  for(int i=0 ; i<DEVICE_COUNT ; i++)
    deviceStatus[i] = false;
  lastPing = millis();
}

void PingHandler::update (){
  if (millis() - lastPing < 1000)
    return;

  lastPing = millis();
  
  for (int i=0 ; i<DEVICE_COUNT ; i++){
    //Serial.print("Start pinging: "); Serial.println(ip[i]);
    deviceStatus[i] = ping_start(ip[i], 1, 1, 32, 100);
  }

}
  
PingHandler pingHandler;

/* Change line 292, 293 in ping.cpp from:
// Timeout
  tout.tv_sec = timeout;
  tout.tv_usec = 0;
*/

/* to this:
// Timeout
  tout.tv_sec = 0;
  tout.tv_usec = timeout*1000;
*/
