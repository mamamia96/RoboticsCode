// MotorControl.cpp

//we want this to control individual motors


#include "MotorControl.h"

MotorControl::MotorControl(Adafruit_MotorShield* AFMS, int motorNum, double tickPerRev){

  this->motor = AFMS->getMotor(motorNum);
  this->distancePerTick = 204.04644/tickPerRev;
  
}

void MotorControl::setDirection(int dir){
  if(dir == 1){
    this->dir = FORWARD;
  }else if(dir == 0){
    this->dir = BACKWARD;
  }
}

int MotorControl::getDirection(){
  return this->dir;
}

void MotorControl::startRun(){
  this->motor->run(this->getDirection());
}

void MotorControl::setMotorSpeed(int spd){
  this->motor->setSpeed(spd);
}



//every motor has an encoder
//encoder has identical pulse widths to wheel ticks
// waves are offset to determine direction of wheels
// TODO: manualy measure how many ticks each wheel takes to make a full revolution
// one tick represents the change from high to low from encoder
// any change in wave is one tick!
