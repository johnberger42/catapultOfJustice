
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>
#include <Servo.h>

Servo lock;
Servo aim;

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
const int pullSteps = 200*3/4;

// initialize the stepper library on pins 7 to 10:
Stepper myStepper(stepsPerRevolution, 7, 8, 9, 10);

bool loaded = false;
bool tracking = false;
unsigned long sawFace = 0;

const int middle = 300 / 2;

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(5);
  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("Startup");
  lock.attach(6);
  lock.write(90);

  aim.attach(5);
  aim.write(90);

}

void loop() {
  if(!loaded) {
    loaded = true;
    // step one revolution  in one direction:
    Serial.println("clockwise");
    myStepper.step(pullSteps);
    lock.write(140);
    delay(200);
  
  
    // step one revolution in the other direction:
    Serial.println("counterclockwise");
    myStepper.step(-pullSteps);    
  }
  int pos;
  while(Serial.available() < 1) { 
    aim.write(90);
    tracking = false;
  }
  pos = map(Serial.read(), 0, 300, 0, 180);
  aim.write(pos);
  if(pos != 90 && !tracking) {
    tracking = true;
    sawFace = millis();
  }
  else if(pos == 90 && tracking) {
    tracking = false;
  }

  if(tracking && (millis() - sawFace > 1000ul)) {
    lock.write(90);
    tracking = false;
  }

  delay(300);
}
