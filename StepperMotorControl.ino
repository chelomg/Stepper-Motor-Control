// motor driver: A4988
// stepper motor: JK42HS60-1204AC
 
// Include the AccelStepper library:
#include <MultiStepper.h>
#include <accelstepper.h>

// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

// speed
int motor_speed = 200;
// axis
int x_axis = 1;

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
    String str = Serial.readStringUntil('/n');
    if(str[0] == 's'){
      motor_speed = getSerialSpeed(str); 
      Serial.println("set speed: "+String(motor_speed)+" mm/sec");
    }else if(str[0] == 'd'){
      x_axis = getSerialPosition(str);
      runMotor(motor_speed, x_axis);
    }
  }
}

int mmToStep(int mm){
  int step = mm*5;
  return step;
}

void runMotor(int motor_speed, int x_axis){
  Serial.println("using "+String(motor_speed)+" mm/sec to shift x: "+String(x_axis));
  //stepper.setCurrentPosition(0);
  
  // mm/sec -> step/sec
  int end_position = mmToStep(x_axis);
  int motor_speed_per_steps = mmToStep(motor_speed);
  
  Serial.println(stepper.currentPosition());
  if(end_position > stepper.currentPosition()){
    motor_speed_per_steps = abs(motor_speed_per_steps);
  }else{
    motor_speed_per_steps = -1 * abs(motor_speed_per_steps);
  }
  
  while(stepper.currentPosition() != end_position)
  {
    stepper.setSpeed(motor_speed_per_steps);
    stepper.runSpeed();
  }
}

int getSerialPosition(String str){
  
  String x_axis_s = str.substring(1, str.length()-1);
  
  // pos, string to int to steps
  int x_axis_i = x_axis_s.toInt();
  
  return x_axis_i;
}

int getSerialSpeed(String str){
  String speed_s = str.substring(1, str.length()-1);
  
  // speed, string to int to steps
  int speed_i = speed_s.toInt();
  
  return speed_i;
}

// 200 steps -> 1 full cycle = 40 mm

// 200 steps = 40 mm, 1 steps = 0.2mm, 5 steps = 1mm
// max: 1000 steps/1 sec = 200 mm/sec

// s200d40 = speed: 200 mm/sec, distance: 40mm
