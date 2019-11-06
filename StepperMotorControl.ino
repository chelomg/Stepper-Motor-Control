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
float motor_speed = 70;
// axis
struct coordinate {
  float x = 1;
  float y = 1;
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
  motor_speed = getSerialSpeed("s300");
}

void loop() {
  /*
  if ( Serial.available() > 0 ) {
    String str = Serial.readStringUntil('/n');
    if(str[0] == 's'){
      motor_speed = getSerialSpeed(str); 
      Serial.println("set speed: "+String(motor_speed)+" mm/sec");
    }else if(str[0] == 'd'){
      runMotor(motor_speed, getSerialPosition(str));
    }
  }*/
  
  
  
  delay(4000);
  // 1
  runMotor(motor_speed, getSerialPosition("d0,10.0"));
  delay(2000);
  runMotor(motor_speed, getSerialPosition("d0,0"));
  delay(4000);
  // 2
  runMotor(motor_speed, getSerialPosition("d0,10.0"));
  delay(2000);
  runMotor(motor_speed, getSerialPosition("d0,0"));
  delay(4000);
  // 3
  runMotor(motor_speed, getSerialPosition("d,5.0"));
  delay(2000);
  runMotor(motor_speed, getSerialPosition("d0,0"));
  
  
  /*
  delay(4000);
  // 1
  runMotor(motor_speed, getSerialPosition("d0,10.0"));
  delay(2000);
  runMotor(motor_speed, getSerialPosition("d0,0"));
  delay(4000);
  // 2
  runMotor(motor_speed, getSerialPosition("d0,10.0"));
  delay(2000);
  runMotor(motor_speed, getSerialPosition("d0,0"));
  delay(4000);
  // 3
  runMotor(motor_speed, getSerialPosition("d-7.07,7.07"));
  delay(2000);
  runMotor(motor_speed, getSerialPosition("d0,0"));
  */
   
}

int mmToStep(float mm){
  int step = mm*10;
  return step;
}

void runMotor(int motor_speed, struct coordinate pos){
  Serial.println("using "+String(motor_speed)+" mm/sec to shift x: "+String(pos.x)+" and y: "+String(pos.y));
  
  // motor X
  // mm/sec -> step/sec
  float x_end_position = mmToStep(pos.x);
  float x_motor_speed_per_steps = mmToStep(motor_speed);
  
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
  float x_axis_i = x_axis_s.toFloat();
  float y_axis_i = y_axis_s.toFloat();
  
  struct coordinate pos;
  pos.x = x_axis_i;
  pos.y = y_axis_i;
  
  return pos;
}

float getSerialSpeed(String str){
  String speed_s = str.substring(1, str.length()-1);
  
  // speed, string to int to steps
  float speed_f = speed_s.toFloat();
  
  return speed_f;
}

struct coordinate adjustPosition(coordinate pos){
  int x_i = floor(pos.x * 10);
  int y_i = floor(pos.y * 10);

  if((x_i & 0x01) != 0){
    if(x_i != 0){
      x_i += 1;
      pos.x = x_i / 10.0;
    }
  }

  if((y_i & 0x01) != 0){
    if(y_i != 0){
      y_i += 1;
      pos.y = y_i / 10.0;
    }  
  }

  return pos;
}

// 1/2 step mode

// 200 steps -> 1 full cycle = 40 mm

// 200 steps = 40 mm, 1 steps = 0.2mm, 5 steps = 1mm
// max: 1000 steps/1 sec = 200 mm/sec

// s200d40 = speed: 200 mm/sec, distance: 40mm
