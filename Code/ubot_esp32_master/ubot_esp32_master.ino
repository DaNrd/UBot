/*
   Code for UBot - Master ESP
   2020 © Aaron Campbell
   Heavily inspired by Gutierrez PS <https://github.com/gutierrezps>
*/

#include <Arduino.h>
#include <Wire.h>
#include <WireSlaveRequest.h>
#include <WiFi.h>

/*
   Pin Assignments
*/
#define SDA_PIN 26
#define SCL_PIN 27
#define I2C_SLAVE1_ADDR 0x04
#define I2C_SLAVE2_ADDR 0x05

/*
   Network Information
*/
const char* ssid = "ubot";
const char* password = "";

/*
   Constants
*/
const int requestInterval = 001;

/*
   Global Variables
*/
/*
   set the max number of bytes the slave will send.
   if the slave send more bytes, they will still be read
   but the WireSlaveRequest will perform more reads
   until a whole packet is read
*/
#define MAX_SLAVE_RESPONSE_LENGTH 32
unsigned long lastConnectionTime = 0;
int measurementNumber = 0;
// create 2 WireSlaveRequest objects (one for each of the 2 slaves)
WireSlaveRequest slaveReq1(Wire, I2C_SLAVE1_ADDR, MAX_SLAVE_RESPONSE_LENGTH);
WireSlaveRequest slaveReq2(Wire, I2C_SLAVE2_ADDR, MAX_SLAVE_RESPONSE_LENGTH);




/*
   Begin a new HardwareSerial
*/
//HardwareSerial Serial2(2);

void setup()
{
  // begin a serial monitor on baud 115200
  Serial.begin(115200);

  // begin a second serial for UART to the main board
  // Serial.begin(Baud Rate, Data Protocol,Txd pin, Rxd pin); (pins: 0-31, 0-39)
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  // establish a wifi connection to the remote
  connectWiFi();

  // join I2C bus with ESP32's as master
  Wire.begin(SDA_PIN, SCL_PIN);

}

void connectWiFi() {

  while (WiFi.status() != WL_CONNECTED) {

    WiFi.begin(ssid, password);
    delay(3000);

  }

  // display a notification that the connection is successful
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
  static unsigned long lastReadMillis = 0;

  // request data from slaves every 1000 ms
  if (millis() - lastReadMillis > requestInterval) {

    int masterStrength = 0;
    int slave1Val = 0;
    int slave2Val = 0;

    // In each loop, make sure there is an Internet connection.
    if (WiFi.status() == WL_CONNECTED) {



      // optional: set delay in milliseconds between retry attempts.
      // the default value is 10 ms
      slaveReq1.setRetryDelay(5);
      slaveReq2.setRetryDelay(5);

      // attempts to read a packet from the ESP32 slaves
      bool success1 = slaveReq1.request();
      bool success2 = slaveReq2.request();

      // print the success of the previous step (for debugging, can be removed later)
      Serial.print(success1);
      Serial.print(success2);


      if (success1) {

        /*
           loop through all but the last byte
           receive each byte as a character
           print the character
        */
        while (slaveReq1.available() > 1) {

          char c = slaveReq1.read();
          Serial.print(c);

        }

        // receive the byte as an integer
        slave1Val = slaveReq1.read();

        // print the strength of slave 1's wifi connection
        Serial.print("Slave1: ");
        Serial.print(slave1Val);

      } else {

        // notify if connection was not available
        Serial.println("slave1 not found");

      }

      if (success2) {

        /*
           loop through all but the last byte
           receive each byte as a character
           print the character
        */
        while (1 < slaveReq2.available()) {

          char c = slaveReq2.read();
          Serial.print(c);

        }

        // receive the byte as an integer
        slave2Val = slaveReq2.read();

        // print the strength of slave 2's wifi connection
        Serial.print("Slave2: ");
        Serial.print(slave2Val);

      } else {

        // notify if connection was not available
        Serial.println("slave2 not found");

      }

      // print the strength of the master's wifi connection
      Serial.print("Master: ");
      masterStrength = getStrength(7);
      Serial.println(masterStrength);

      // send data to the main board
      String message = String(masterStrength) + "," + String(slave1Val) + "," + String(slave2Val);
//      Serial2.println(message);
      Serial2.write(masterStrength);
      Serial2.write(slave1Val);
      Serial2.write(slave2Val);
      Serial.println();
      Serial.println(message);
      Serial.println();
//      delay(600-140);

    } else {
      Serial.println("noWifi");
      Serial2.write(0);
      Serial2.write(0);
      Serial2.write(0);
//      delay(200);
    }

    lastReadMillis = millis();

  }

}
