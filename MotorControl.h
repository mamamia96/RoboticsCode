// MotorControl.h

#ifndef MotorControl_h
#define MotorControl_h

#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>


class MotorControl{

  private:
    Adafruit_DCMotor* motor;
    uint8_t dir;
    double distancePerTick;
  public:
    MotorControl(Adafruit_MotorShield* AFMS, int motorNum, double tickPerRev);
    void setDirection(int dir);
    int getDirection();
    void startRun();
    void setMotorSpeed(int spd);

  
};

#endif
