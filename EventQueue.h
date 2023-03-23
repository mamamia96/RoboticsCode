#include "Event.h"

#ifndef EventQueue_h
#define EventQueue_h

#define EVENT_LIMIT 5

class EventQueue{
    private:
        Event *events[EVENT_LIMIT];


    public:
        int eventNum = 0;
        void addEvent(int aSpeed, int bSpeed, int aTicks, int bTicks, int aDir, int bDir);
        bool update(int leftEncodeA, int leftEncodeB, int rightEncodeA, int rightEncodeB);
        void getEventInfo(int info[]);
        Event * getCurrentEvent();
        bool isEmpty();
};


#endif