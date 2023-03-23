/*
==MultiMotor Class File==
This class:
 - controls instances of MotorControl classes (which directly interface with DC motors)
 - controls an instance of EventQueue class which queues events for the motors (turning, driving, etc)
*/

#include "MultiMotor.h"

MultiMotor::MultiMotor(Adafruit_MotorShield* AFMS){

    double d = 5;

    for(int i = 0; i < 2; i++){
        // this->motorControls[i] = new MotorControl(&AFMS, i+1, d);
        this->motorControls[i] = new MotorControl(AFMS, i+1, d);
    }
    this->eventQueue = new EventQueue();
}

void MultiMotor::addEvent(int aSpeed, int bSpeed, int aTicks, int bTicks, int aDir, int bDir){
    //function for adding a new event to MultiMotor's event queue 
    this->eventQueue->addEvent(aSpeed, bSpeed, aTicks, bTicks, aDir, bDir);

    //if an event was added to an empty queue we start that event
    if(this->eventQueue->eventNum == 1) this->startNextEvent();
}

void MultiMotor::startNextEvent(){
    //function for starting the next event in MultiMotor's EventQueue 
    if(this->eventQueue->eventNum <= 0){
        this->reset();
        return;
    }

    //getting information from event about what to do with motors
    //WITH POINTERS
    int info[4];
    this->eventQueue->getCurrentEvent()->getInfo(info);


    //motor A
    this->setSpeed(info[0], 0);
    this->setDirection(info[2], 0);


    //motor B
    this->setSpeed(info[1], 1);
    this->setDirection(info[3], 1);

    //telling the motors to start running in their direction (determined with MotorControl's class attribute dir)
    this->startRun();
}

void MultiMotor::setDirection(int dir, int n){
    this->motorControls[n]->setDirection(dir);
}

void MultiMotor::setSpeed(int spd, int n){
    //function for setting motor n's speed
    this->motorControls[n]->setMotorSpeed(spd);
}

void MultiMotor::startRun(){
    for(auto mc: this->motorControls){
        mc->startRun();
    }
}

bool MultiMotor::update(int leftEncodeA, int leftEncodeB, int rightEncodeA, int rightEncodeB){
    //main update loop for the class
    //returns true if the current event is finished
    //returns false if the event is still in progress
    bool eventFinished = this->eventQueue->update(leftEncodeA, leftEncodeB, rightEncodeA, rightEncodeB);

    if(eventFinished){
        this->startNextEvent();
        return true;
    }else{
        return false;
    }
}

void MultiMotor::reset(){
    //function for reseting directions and speed for each motor (so it stops any behavior after finishing its events)
    for(int i = 0; i < 2; i++){
        this->setSpeed(0, i);
        this->setDirection(1, i);
    }
    this->startRun();
}