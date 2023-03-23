/*
==Event Class File==
This class is for updating individual events based on their attributes determined of function construction
*/
#include "Event.h"
#include <Arduino.h>


Event::Event(int aSpeed, int bSpeed, int aTicks, int bTicks, int aDir, int bDir){
    this->aSpeed = aSpeed;
    this->bSpeed = bSpeed;
    this->aTicks = aTicks;
    this->bTicks = bTicks;
    this->aDir   = aDir;
    this->bDir   = bDir;
}


bool Event::update(int leftEncodeA, int leftEncodeB, int rightEncodeA, int rightEncodeB){
    //update function for individual events. If aTicks or bTicks is set to -1 then we can ignore checking its value 
    //for now im going to ignore the B encoders (leftEncodeB and rightEncodeB)

    bool aFinished = (leftEncodeA  >= this->aTicks || this->aTicks == -1);
    bool bFinished = (rightEncodeA >= this->bTicks || this->bTicks == -1);
    return (aFinished && bFinished);
}

void Event::getInfo(int info[]){
    //returns information about the current event using a pointer to an array of integers
    info[0] = this->aSpeed;
    info[1] = this->bSpeed;
    info[2] = this->aDir;
    info[3] = this->bDir;
}