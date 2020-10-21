/*
Code for UBot
2020 © Aaron Campbell
*/

/*
Pin Assignments
*/
const byte MOTOR_L_POSITIVE_PIN = 2; // TODO: assign correct pin
const byte MOTOR_L_NEGATIVE_PIN = 3; // TODO: assign correct pin
const byte MOTOR_L_PWM_PIN = 4; // TODO: assign correct pin
const byte MOTOR_R_POSITIVE_PIN = 5; // TODO: assign correct pin
const byte MOTOR_R_NEGATIVE_PIN = 6; // TODO: assign correct pin
const byte MOTOR_R_PWM_PIN = 7; // TODO: assign correct pin

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
  pinMode(MOTOR_L_POSITIVE_PIN,OUTPUT);
  pinMode(MOTOR_L_NEGATIVE_PIN,OUTPUT);
  pinMode(MOTOR_L_PWM_PIN,OUTPUT);
  pinMode(MOTOR_R_POSITIVE_PIN,OUTPUT);
  pinMode(MOTOR_R_NEGATIVE_PIN,OUTPUT);
  pinMode(MOTOR_R_PWM_PIN,OUTPUT);

  // set initial motor direction to 'positive'
  digitalWrite(MOTOR_L_POSITIVE_PIN,HIGH);
  digitalWrite(MOTOR_L_NEGATIVE_PIN,LOW);
  digitalWrite(MOTOR_R_POSITIVE_PIN,LOW);
  digitalWrite(MOTOR_R_NEGATIVE_PIN,LOW);

  // testing functions
  motor('R',9);
  motor('L',9);
}

void loop() {
}

void motor(char side, float value) {
  int pwmOutput = map(abs(value), 0 , 99, 0, 255); // map value from 0 to 255 //TODO: change 1023 to actual useful value
  if(side == 'R'){
    if(value>0){
      digitalWrite(MOTOR_R_POSITIVE_PIN,HIGH);
      digitalWrite(MOTOR_R_NEGATIVE_PIN,LOW);
    } else if(value<0){
      digitalWrite(MOTOR_R_POSITIVE_PIN,LOW);
      digitalWrite(MOTOR_R_NEGATIVE_PIN,HIGH);
    }
    analogWrite(MOTOR_R_PWM_PIN,pwmOutput);
    Serial.print("MOTOR_R = ");
    Serial.println(pwmOutput);
  } else if(side == 'L'){
    if(value>0){
      digitalWrite(MOTOR_L_POSITIVE_PIN,HIGH);
      digitalWrite(MOTOR_L_NEGATIVE_PIN,LOW);
    } else if(value<0){
      digitalWrite(MOTOR_L_POSITIVE_PIN,LOW);
      digitalWrite(MOTOR_L_NEGATIVE_PIN,HIGH);
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
