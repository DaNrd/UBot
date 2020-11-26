/*
  Code for UBot - Slave ESP
  2020 © Aaron Campbell
  Heavily inspired by Gutierrez PS <https://github.com/gutierrezps>
*/

#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>
#include <WiFi.h>

/*
  Pin Assignments
*/
#define SDA_PIN 26
#define SCL_PIN 27
#define I2C_SLAVE_ADDR 0x04

/*
   Network Information
*/
const char* ssid = "ubot";
const char* password = "";

/*
   Global Variables
*/
unsigned long lastConnectionTime = 0;
int measurementNumber = 0;

void setup()
{
  // begin a serial monitor on baud 115200
  Serial.begin(115200);

  // establish a wifi connection to the remote
  connectWiFi();

  // join I2C bus with ESP32's as slave
  bool res = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
  if (!res) {
    Serial.println("I2C slave init failed");
    while (1) delay(100);
  }

  // define onRequest action
  WireSlave.onRequest(requestEvent);
}

void connectWiFi() {

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(3000);
  }

  // Display a notification that the connection is successful.
  Serial.println("Connected");

}

int getStrength(int points) {

  long rssi = 0;
  int averageRSSI = 0;

  // compute average of a few wifi strengths to midigate large jumps in value
  for (int i = 0; i < points; i++) {
    rssi += WiFi.RSSI();
    delay(20);
  }

  averageRSSI = rssi / points;
  // return the wifi strength as a positive integer
  return -averageRSSI;

}

void loop()
{
  /*
     the slave response time is directly related to how often
     this update() method is called, so avoid using long delays
     inside loop()
  */
  WireSlave.update();

  // let I2C and other ESP32 peripherals interrupts work
  delay(1);
}

// called when master ESP32 asks for something, sends wifi strength to the master
void requestEvent()
{
  Serial.println("Requested.");
  const int numberPoints = 7;
  float wifiStrength;

  // In each loop, make sure there is an Internet connection.
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  // calculate the wifi strength of this slave
  byte x = getStrength(numberPoints);

  // send wifi strength to the master ESP32
  WireSlave.print("Slave1: ");
  WireSlave.write(x);
}
