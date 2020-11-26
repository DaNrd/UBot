/*
   Code for UBot - Remote
   2020 © Aaron Campbell
*/

#include <WiFi.h>

/*
   Network Information
*/
const char *ssid = "ubot";
const char *password = "";

void setup() {

  // begin a serial monitor on baud 115200
  Serial.begin(115200);

  // display setup info
  Serial.println();
  Serial.println("Configuring access point...");
  
  //WiFi.enableAP(true); // Changing ESP32 wifi mode to AccessPoint
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);                //Default IP is 192.168.4.1

  Serial.println("Scan For Wifi in your Mobile or laptop, you will see this network");
}

void loop() {}
