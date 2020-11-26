/*
   Code for UBot
   2020 © Aaron Campbell
*/

/*
   Pin Assignments
*/
const byte MOTOR_L_DIRECTION_PIN = 12; // TODO: assign correct pin
const byte MOTOR_L_PWM_PIN = 10; // TODO: assign correct pin
const byte MOTOR_R_DIRECTION_PIN = 13; // TODO: assign correct pin
const byte MOTOR_R_PWM_PIN = 11; // TODO: assign correct pin
const byte LIDAR_INTERRUPT_PIN = 2;

/*
   Constants
*/
const int motorDuration = 500;

/*
   timeUnit will be used to turn the robot off
   (stop following and return to home) when the
   time limit has been reached
*/
const int timeUnit = 30 * 60000; // min*(ms/min) = ms (currently 30min)

/*
   Global Variables
*/
int NumTime = 0; // length of time paid for
int TimeZero = 0; // time when paid for

void setup() {
  // begin serial monitor on baud 9600
  Serial.begin(9600);

  // begin serial monitor on baud 115200 for UART
  Serial2.begin(9600);

  // declare pin types
  pinMode(MOTOR_L_DIRECTION_PIN, OUTPUT);
  pinMode(MOTOR_L_PWM_PIN, OUTPUT);
  pinMode(MOTOR_R_DIRECTION_PIN, OUTPUT);
  pinMode(MOTOR_R_PWM_PIN, OUTPUT);

  // set initial motor direction to 'positive'
  digitalWrite(MOTOR_L_DIRECTION_PIN, HIGH);
  digitalWrite(MOTOR_R_DIRECTION_PIN, LOW);

  // testing functions
//   motor('R',100);
//   motor('L',9);

  // add interrupt for distance sensor
  pinMode(LIDAR_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LIDAR_INTERRUPT_PIN), lidar_interrupt, LOW);
}

void loop() {
  action(readInput());
//  delay(200);
}

int *readInput() {
  static int retval[] = {0,0,0};
  retval[0] = 0;
  retval[1] = 0;
  retval[2] = 0;
  if (Serial2.available()) {
    retval[0] = Serial2.read();
    retval[1] = Serial2.read();
    retval[2] = Serial2.read();
  }
  return retval;
}

void action(int input[]) {

    int m = input[0];
    int s1 = input[1];
    int s2 = input[2];

  Serial.println("Values: ");
  Serial.println(m);
  Serial.println(s1);
  Serial.println(s2);


  Serial.println();
      Serial.println("forward");
      Serial.println();
  forward(m);

  // below code should compute turns and stuff
  
//  int backPower = ((s1 + s2) / 2);
//  if (m >= backPower) {
//    if (s1 > (s1+s2)/2) {
//      Serial.println();
//      Serial.println("turn false");
//      Serial.println();
//      rotate(false);
//    } else if ((s1+s2)/2 < s2) {
//      Serial.println();
//      Serial.println("turn true");
//      Serial.println();
//      rotate(true);
//    } else {
//      Serial.println();
//      Serial.println("forward");
//      Serial.println();
//      forward(m);
//    }
//  } else {
//    Serial.println();
//      Serial.println("turn true");
//      Serial.println();
//    rotate(true);
//  }

}

// rotates the bot in a direction (true for right, false for left)
void rotate(boolean direc) {
  if (direc) {
    runMotors(-30, 30, motorDuration);
  } else {
    runMotors(30, -30, motorDuration);
  }
}

// moves the bot forward
void forward(int power) {

  int minDist = 30;
  int maxDist = 50;

  int usedPower = power;
  if (power > maxDist) {
    usedPower = maxDist;
  } else if (power < minDist) {
    usedPower = minDist;
  }
  Serial.println(power);
//  int speedInt = map(power, minDist, maxDist, 0, 100); // map power to 0-100
  int speedInt = (minDist-usedPower)*5;
  runMotors(-speedInt, -speedInt, motorDuration);
}

// runs both motors for duration then stops
void runMotors(int l, int r, int duration) {
  motor('R', r);
  motor('L', l);
  delay(200);
//  delay(duration);
//  motor('R', 0);
//  motor('L', 0);
}

// a function to interface with the motors
void motor(char side, float value) {
  int pwmOutput = map(abs(value), 0 , 100, 0, 255); // map value from 0 to 255 //TODO: change 1023 to actual useful value
  if (side == 'R') {
    if (value > 0) {
      digitalWrite(MOTOR_R_DIRECTION_PIN, HIGH);
    } else if (value < 0) {
      digitalWrite(MOTOR_R_DIRECTION_PIN, LOW);
    }
    analogWrite(MOTOR_R_PWM_PIN, pwmOutput);
    Serial.print("MOTOR_R = ");
    Serial.println(pwmOutput);
  } else if (side == 'L') {
    if (value > 0) {
      digitalWrite(MOTOR_L_DIRECTION_PIN, HIGH);
    } else if (value < 0) {
      digitalWrite(MOTOR_R_DIRECTION_PIN, HIGH);
    }
    analogWrite(MOTOR_L_PWM_PIN, pwmOutput);
    Serial.print("MOTOR_L = ");
    Serial.println(pwmOutput);
  } else {
    Serial.println("/*ERROR*/  Function motor() invalid char: side");
    Serial.print("/*ERROR*/  Expected: \'L\'' or \'R\', received: \'");
    Serial.print(side);
    Serial.println("\'");
  }
}

void lidar_interrupt() {
  analogWrite(MOTOR_R_PWM_PIN, 0);
  analogWrite(MOTOR_L_PWM_PIN, 0);
}
