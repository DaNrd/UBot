/*
   Code for UBot - Lidar
   2020 © Aaron Campbell
*/

#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <Servo.h>

/*
   Pin Assignments
*/
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3
#define STOP_PIN 2

/*
   Constants
*/
#define DELAY 20
#define MIN_ANGLE 30
#define MAX_ANGLE 150
#define STEP_ANGLE 10
#define TRIG_DIST 1300 // in mm

/*
   Global Variables
*/
int pos = 0;    // variable to store the servo position
int distance = 0;


SFEVL53L1X distanceSensor;
Servo myservo;


bool lidar_mes() {

  distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  
  if (distance >= TRIG_DIST) {
    digitalWrite(STOP_PIN, HIGH);
  }
  else {
    digitalWrite(STOP_PIN, LOW);
  }
  
}



void setup(void)
{
  // begin a wire connection to board
  Wire.begin();

  // begin serial monitor on baud 115200
  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  // complain if sensor does not begin
  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }

  // begin sensing distance
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }

  // report successful connection
  Serial.println("Sensor online!");
  
  // attach to scanning servo
  myservo.attach(3);
}



void loop(void)
{
  for (pos = MIN_ANGLE; pos <= MAX_ANGLE; pos += STEP_ANGLE) { // goes from 0 degrees to 180 degrees
    lidar_mes();
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(DELAY);
  }
  for (pos = MAX_ANGLE; pos >= MIN_ANGLE; pos -= STEP_ANGLE) { // goes from 180 degrees to 0 degrees
    lidar_mes();
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(DELAY);
  }
}
