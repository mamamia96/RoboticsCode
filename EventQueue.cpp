/*
==EventQueue File==
This class is basically a queue of Event Class files. It's intended to be the child of a MultiMotor class
*/
#include "EventQueue.h"
#include "Arduino.h"


void EventQueue::addEvent(int aSpeed, int bSpeed, int aTicks, int bTicks, int aDir, int bDir){
    //add new event to the queue with directions and speeds for both motors
    this->events[this->eventNum] = new Event(aSpeed, bSpeed, aTicks, bTicks, aDir, bDir);
    this->eventNum++;
}


bool EventQueue::update(int leftEncodeA, int leftEncodeB, int rightEncodeA, int rightEncodeB){
    //main update function for EventQueue
    //checks to see if the current event is finished and starts a new one if applicable

    if(this->eventNum <= 0) return false;

    if (this->events[0]->update(leftEncodeA, leftEncodeB, rightEncodeA, rightEncodeB)){
        //event is finished
        for(int i = 1; i < EVENT_LIMIT; i++){
            this->events[i-1] = this->events[i];
        }
        this->eventNum--;
        return true;
    }
    return false;
}

void EventQueue::getEventInfo(int info[]){
    //'returns' information about the curretn event using a pointer to an int array
    this->events[this->eventNum]->getInfo(info);
}

Event * EventQueue::getCurrentEvent(){
    //returns a pointer the 'next up' event in queue
    if(this->eventNum <= 0) return nullptr;
    return this->events[0];
}

bool EventQueue::isEmpty(){
    return (this->eventNum == 0);
}
