#ifndef MultiMotor_h
#define MultiMotor_h



#include "MotorControl.h"
#include "EventQueue.h"


#define EVENT_LIMIT 5

class MultiMotor{
    private:
        MotorControl *motorControls[2];
        

    public:
//        MultiMotor();
        EventQueue *eventQueue;
        MultiMotor(Adafruit_MotorShield* AFMS);
        void setDirection(int dir, int n);
        void setSpeed(int spd, int n);
        void startRun();
        void addEvent(int aSpeed, int bSpeed, int aTicks, int bTicks, int aDir, int bDir);
        bool update(int leftEncodeA, int leftEncodeB, int rightEncodeA, int rightEncodeB);
        void startNextEvent();
        void reset();
};


#endif