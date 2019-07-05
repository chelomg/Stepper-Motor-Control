// motor driver: A4988
// stepper motor: JK42HS60-1204AC
 
// Include the AccelStepper library:
#include <MultiStepper.h>
#include <accelstepper.h>

// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

// speed
int speed_int = 1;
String speed_string = "";
// distance
int dis_int = 1;
String dis_string = "";

#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  
  Serial.begin(9600);
  stepper.setMaxSpeed(1000);
  stepper.setCurrentPosition(400);
}

void loop() {
  if ( Serial.available() > 0 ) {
    // useless
    speed_string = Serial.readStringUntil('s');

    // get speed string
    speed_string = Serial.readStringUntil('d');
    // get distance string
    dis_string = Serial.readStringUntil('/n');
    Serial.println("speed: "+speed_string+" mm/sec, distance: "+dis_string);
    
    // speed, string to int 
    speed_int = speed_string.toInt();
    speed_int = mmToStep(speed_int);
    // distance, string to int 
    dis_int = dis_string.toInt();
    dis_int = mmToStep(dis_int);
    
    stepper.setCurrentPosition(0);
    while(stepper.currentPosition() != dis_int)
    {
      stepper.setSpeed(speed_int);
      stepper.runSpeed();
    }
  }
}

int mmToStep(int mm){
  int step = mm*5;
  return step;
}

// 200 steps -> 1 full cycle = 40 mm

// 200 steps = 40 mm, 1 steps = 0.2mm, 5 steps = 1mm
// max: 1000 steps/1 sec = 200 mm/sec

// s200d40 = speed: 200 mm/sec, distance: 40mm
