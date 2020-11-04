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

/*
Constants
*/
const int timeUnit = 30*60000; // min*(ms/min) = ms (currently 30min)

/*
Global Variables
*/
int NumTime = 0; // length of time paid for
int TimeZero = 0; // time when paid for

void setup() {
  // begin serial monitor on band 9600
  Serial.begin(9600);

  // declare pin types
  pinMode(MOTOR_L_DIRECTION_PIN,OUTPUT);
  pinMode(MOTOR_L_PWM_PIN,OUTPUT);
  pinMode(MOTOR_R_DIRECTION_PIN,OUTPUT);
  pinMode(MOTOR_R_PWM_PIN,OUTPUT);

  // set initial motor direction to 'positive'
  digitalWrite(MOTOR_L_DIRECTION_PIN,HIGH);
  digitalWrite(MOTOR_R_DIRECTION_PIN,LOW);

  // testing functions
//  motor('R',100);
//  motor('L',9);
}

void loop() {.  
  readInput();
}

void readInput() {
  String received = "";
  if(Serial.available()) {
    received += char(Serial.read());   // read one byte from serial buffer and save to data_rcvd
  }
  if(received.length() != 0){
    motor('R',100);
  }
}

void motor(char side, float value) {
  int pwmOutput = map(abs(value), 0 , 100, 0, 255); // map value from 0 to 255 //TODO: change 1023 to actual useful value
  if(side == 'R'){
    if(value>0){
      digitalWrite(MOTOR_R_DIRECTION_PIN,HIGH);
    } else if(value<0){
      digitalWrite(MOTOR_R_DIRECTION_PIN,LOW);
    }
    analogWrite(MOTOR_R_PWM_PIN,pwmOutput);
    Serial.print("MOTOR_R = ");
    Serial.println(pwmOutput);
  } else if(side == 'L'){
    if(value>0){
      digitalWrite(MOTOR_L_DIRECTION_PIN,HIGH);
    } else if(value<0){
      digitalWrite(MOTOR_R_DIRECTION_PIN,HIGH);
    }
    analogWrite(MOTOR_L_PWM_PIN,pwmOutput);
    Serial.print("MOTOR_L = ");
    Serial.println(pwmOutput);
  } else {
    Serial.println("/*ERROR*/  Function motor() invalid char: side");
    Serial.print("/*ERROR*/  Expected: \'L\'' or \'R\', received: \'");
    Serial.print(side);
    Serial.println("\'");
  }
}
