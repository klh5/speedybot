/* Pins 2 and 13 are available, as well as all analogue pins */
/* Servos are on digital pins 9 and 10 */
 
//Libraries
#include <AFMotor.h>
#include <Servo.h>

//Set parameters for servos
#define PAN_MID 90
#define PAN_RIGHT 0
#define PAN_LEFT 180

//Define pins for right ultrasonic sensor
#define ECHO_RIGHT A0
#define TRIG_RIGHT A1

//Define pins for left ultrasonic sensor
#define ECHO_LEFT A3
#define TRIG_LEFT A2

//Set up motors - speedybot has four
AF_DCMotor frontRight(1, MOTOR12_64KHZ);
AF_DCMotor frontLeft(2, MOTOR12_64KHZ);
AF_DCMotor backRight(3, MOTOR12_64KHZ);
AF_DCMotor backLeft(4, MOTOR12_64KHZ);

boolean goBack = false;
Servo pan;

void setup() {

  Serial.begin(9600);
  
  pan.attach(10);
  frontRight.setSpeed(255);
  frontLeft.setSpeed(255);
  backRight.setSpeed(255);
  backLeft.setSpeed(255);

  pan.write(PAN_MID);
}

void loop() {

  go(30000);
  delay(500);
  
}

//Main function that drives movement
void go(int duration) { 

  long a,b;
  int accelVal, ledVal = 0;
  boolean shouldMove = true;
  a = millis();

  do {

    checkForObstacle();
    if(goBack == false) {
      frontRight.run(FORWARD);
      frontLeft.run(FORWARD);
      backRight.run(FORWARD);
      backLeft.run(FORWARD);
    }
    else {     
      getOutOfThere();
      goBack = false;
    }

    b = millis()-a;
    if(b>=duration) {
      shouldMove=false;
    }
  } 
  while(shouldMove!=false);

  stopMoving();
} 

//Check both ultrasonic sensors for distances less than 15cm, which would indicate a collision is likely
void checkForObstacle() {

  int leftDist, rightDist;
  
  leftDist = getDistance(TRIG_LEFT, ECHO_LEFT);
  delay(15);
  rightDist = getDistance(TRIG_RIGHT, ECHO_RIGHT);
    
  if(leftDist < 20 || rightDist < 20) {
    goBack = true;
  }
  
}

//Return a distance from one ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  
  int distance;
  unsigned long pulseDuration = 0; 

  pinMode(trigPin, OUTPUT); // Trigger pin
  pinMode(echoPin, INPUT);  // Echo pin
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  pulseDuration = pulseIn(echoPin,HIGH);
  pulseDuration = pulseDuration/2;
  
  distance = int(pulseDuration/29);

  if(distance > 200) {
    distance = 200;
  }
  
  return distance;
}

//Used when a potential collision is detected. Go backwards, then turn randomly left or right
void getOutOfThere() {
  
  int randDir;
  randDir = random(1,3);

  moveBackward(1000);

  if(randDir == 1) {
    goLeft(500);
  }
  else {
    goRight(500);
  }
}

//Stop motors
void stopMoving() {

  frontRight.run(RELEASE);
  frontLeft.run(RELEASE);
  backRight.run(RELEASE);
  backLeft.run(RELEASE);
 
}

//Reverse motors
void moveBackward(int duration){
  
  frontRight.run(BACKWARD);
  frontLeft.run(BACKWARD);
  backRight.run(BACKWARD);
  backLeft.run(BACKWARD);
  delay(duration);
  
}

//Turn to the left - right motors run forwards, left motors run backwards
void goLeft(int duration){
  
  frontRight.run(FORWARD);
  frontLeft.run(BACKWARD);
  backRight.run(FORWARD);
  backLeft.run(BACKWARD);
  delay(duration);
  
}

//Turn to the right - right motors run backwards, left motors run forwards
void goRight(int duration){
  
  frontRight.run(BACKWARD);
  frontLeft.run(FORWARD);
  backRight.run(BACKWARD);
  backLeft.run(FORWARD);
  delay(duration);
  
}
