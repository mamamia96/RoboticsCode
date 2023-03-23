#include "MiaClasses.h"


#define DEBUG 0 //if this is set to 1 then Serial.print statements for debugging will be executed

// == ENCODER VARIABLES ==
int aTicks = 0;
int bTicks = 0;
#define Encoder_output_A 3 
#define Encoder_output_B 2
#define base_ticks 30 //how many ticks the wheel rotates before checking ir sensors

// == MOTOR VARIABLES ==
#define motor_speed 50
MultiMotor *MM; //my custom class that uses other classes as components
int aMotorSpeed;
int bMotorSpeed;
int aMotorTicks;
int bMotorTicks;

// == TURNING MATH VARIABLES ==
#define theta 5                     //degrees
#define dist_between_wheels  181.0      //mm
#define ticks_per_revolution 328.6     //ticks
#define wheel_circumference  204.04644 //mm
#define correct_angle 4

// == IR SENSOR VARIABLES ==
//IR sensors are in pins A4(leftmost) - A8 (righttmost)
#define IR_0  A4
#define IR_1  A5
#define IR_2  A6
#define IR_3  A7
#define IR_4  A8
#define IR_THRESHOLD 250

int irData[5];
int irPins[] = {IR_4, IR_3, IR_2, IR_1, IR_0};


//variables used for delaying print statements
int printFrame = 1000;
int frameCount = 0;




void setup() {
  if(DEBUG == 1) Serial.begin(9600);

  //start adafruit process
  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
  AFMS.begin();

  //modified speed (used for motor B since it is more powerful than motor A)
  int mod_speed = floor(motor_speed * 0.93808479923007);


  //initializing my class object that directly interfaces with the motors
  MM = new MultiMotor(&AFMS);
  MM->addEvent(motor_speed, mod_speed, 5, 5, true, true);

  //attaching interrupts
  attachInterrupt(digitalPinToInterrupt(Encoder_output_A),A_Motor_Encoder,RISING);
  attachInterrupt(digitalPinToInterrupt(Encoder_output_B),B_Motor_Encoder,RISING);
}

void loop() {
  //we constantly update the multimotor with our encoder values (ticks)
  //if the update returns true that means it is finished its event
  bool eventFinished = MM->update(abs(aTicks), -1, abs(bTicks), -1);

  //updating our array that holds all the ir sensor data
  update_ir();

  //if the current event is finished and there aren't any others queued
  //did not use the queuing functionality of class just to make sure we are updating everything in real time 
  if (eventFinished && MM->eventQueue->isEmpty()){

     //figuring out what direction by reading ir sensors
     int sensor = lineThreshold();

    if(sensor == 2){
      //straight
      aMotorSpeed = motor_speed;
      bMotorSpeed = floor(aMotorSpeed * 0.93808479923007); //adjust motor speed so goes straight
      aMotorTicks = base_ticks;
      bMotorTicks = base_ticks;
      if(DEBUG == 1) Serial.println("CENTERED!");
     
    }else{
      //turning
      if(sensor < 2){
        //turning left
        if(DEBUG == 1) Serial.println("TURNING LEFT!");
        aMotorSpeed = 0;
        bMotorSpeed = motor_speed;
        
        aMotorTicks = -1;
        bMotorTicks = thetaToTicks(correct_angle); //setting to -1 so event handler doesnt bother checking bMotorTicks
        
      }else if(sensor > 2){
        //turning right
        if(DEBUG == 1) Serial.println("TURNING RIGHT!");
        aMotorSpeed = motor_speed;
        bMotorSpeed = 0;
        
        aMotorTicks = thetaToTicks(correct_angle);
        bMotorTicks = -1; //setting to -1 so event handler doesnt bother checking bMotorTicks
      }
    }

    //adding events with variables modified by ir sensor
    MM->addEvent(aMotorSpeed, bMotorSpeed, aMotorTicks, bMotorTicks, true, true);

    //reset our tick values after event if finished
    aTicks = 0;
    bTicks = 0;
  }


//code for printing out debug information
  if(frameCount >= printFrame && DEBUG == 1){
    print_ir();
    
    frameCount = 0;
  }
    frameCount++;

}

void A_Motor_Encoder(){
  //interrupt func for the A motor's encoder
  aTicks++;
}

void B_Motor_Encoder(){
  //interrupt func for the B motor's encoder
  bTicks++;
}



void update_ir(){
  //func for reading ir sensor values into array
  for(int i = 0; i < 5; i++){
    irData[i] = int(analogRead(irPins[i]));
  }
}

void print_ir(){
  //func for printing out values in ir sensor array
  for(int i = 0; i < 5; i++){
    //formatting

    int data = irData[i];
    
    Serial.print(irData[i]);

    if(data < 100){
      Serial.print(" ");
    }else if( data < 10){
      Serial.print("  ");
    }
    
    Serial.print(" | ");
  }
  Serial.println();
}

int lineThreshold(){
  //func for detecting if ir_sensor[n] is under a certain threshold
  //returns n(0-4) for which sensor has the lowest value (sees the black tape most clearly)
  //returns -1 if none of the sensors reach the threshold (IR_THRESHOLD)
  int minVal = 10000;
  int minIndex = -1;
  for(int i = 0; i < 5; i++){
    if(irData[i] <= IR_THRESHOLD && irData[i] < minVal){
      minVal = irData[i];
      minIndex = i;
    }
  }
  return minIndex;
}

int thetaToTicks(int angle){
  //rotating angle degrees
  //one motor is stationary other is traveling totalTicks or dist
  //made using mm but shouldn't matter what units are used

  //adjusting the angle based on weight factor 
  //without the robot is turning only 75% of degrees requested
  
  double dist = ((2 * PI * dist_between_wheels)/360) * angle;
  double totalTicks = (ticks_per_revolution/wheel_circumference) * dist;
  return (int)totalTicks;
}
