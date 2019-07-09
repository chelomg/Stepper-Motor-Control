// motor driver: A4988
// stepper motor: JK42HS60-1204AC
 
// Include the AccelStepper library:
#include <MultiStepper.h>
#include <accelstepper.h>

// stepper motor X pins
const int stepPin = 3; 
const int dirPin = 4; 

// stepper motor Y pins
const int yStepPin = 5;
const int yDirPin = 6;

// speed
int motor_speed = 200;
// axis
struct coordinate {
  int x = 1;
  int y = 1;
};

#define motorInterfaceType 1

// stepper motor X
AccelStepper stepper_x = AccelStepper(motorInterfaceType, stepPin, dirPin);

// stepper motor Y
AccelStepper stepper_y = AccelStepper(motorInterfaceType, yStepPin, yDirPin);

void setup() {
  
  Serial.begin(9600);
  stepper_x.setMaxSpeed(1000);
  stepper_x.setCurrentPosition(0);
  stepper_y.setMaxSpeed(1000);
  stepper_y.setCurrentPosition(0);
}

void loop() {
  if ( Serial.available() > 0 ) {
    String str = Serial.readStringUntil('/n');
    if(str[0] == 's'){
      motor_speed = getSerialSpeed(str); 
      Serial.println("set speed: "+String(motor_speed)+" mm/sec");
    }else if(str[0] == 'd'){
      runMotor(motor_speed, getSerialPosition(str));
    }
  }
}

int mmToStep(int mm){
  int step = mm*5;
  return step;
}

void runMotor(int motor_speed, struct coordinate pos){
  Serial.println("using "+String(motor_speed)+" mm/sec to shift x: "+String(pos.x)+" and y: "+String(pos.y));
  
  // motor X
  // mm/sec -> step/sec
  int x_end_position = mmToStep(pos.x);
  int x_motor_speed_per_steps = mmToStep(motor_speed);
  
  if(x_end_position > stepper_x.currentPosition()){
    x_motor_speed_per_steps = abs(x_motor_speed_per_steps);
  }else{
    x_motor_speed_per_steps = -1 * abs(x_motor_speed_per_steps);
  }
  
  // motor Y
  // mm/sec -> step/sec
  int y_end_position = mmToStep(pos.y);
  int y_motor_speed_per_steps = mmToStep(motor_speed);
  
  if(y_end_position > stepper_y.currentPosition()){
    y_motor_speed_per_steps = abs(y_motor_speed_per_steps);
  }else{
    y_motor_speed_per_steps = -1 * abs(y_motor_speed_per_steps);
  }
  
  // Run
  while(stepper_y.currentPosition() != y_end_position || stepper_x.currentPosition() != x_end_position)
  {
    if(stepper_x.currentPosition() != x_end_position){
      stepper_x.setSpeed(x_motor_speed_per_steps);
      stepper_x.runSpeed();
    }
    
    if(stepper_y.currentPosition() != y_end_position){
      stepper_y.setSpeed(y_motor_speed_per_steps);
      stepper_y.runSpeed();
    }
  }
  
  Serial.println(stepper_x.currentPosition());
  Serial.println(stepper_y.currentPosition());
}

struct coordinate getSerialPosition(String str){
  int break_point = 0;
  
  for(int i = 0; i < str.length(); i++){
    if(str[i] == ','){
      break_point = i;
      break;
    }
  }
  
  String x_axis_s = str.substring(1, break_point);
  Serial.println(x_axis_s);
  String y_axis_s = str.substring(break_point + 1, str.length()-1);
  // pos, string to int to steps
  int x_axis_i = x_axis_s.toInt();
  int y_axis_i = y_axis_s.toInt();
  
  struct coordinate pos;
  pos.x = x_axis_i;
  pos.y = y_axis_i;
  
  return pos;
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
