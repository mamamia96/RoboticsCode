#ifndef Event_h
#define Event_h



class Event{
    private:
        int aSpeed;
        int bSpeed;
        int aTicks;
        int bTicks;
        int aDir;
        int bDir;


    public:
        Event(int aSpeed, int bSpeed, int aTicks, int bTicks, int aDir, int bDir);
        bool update(int leftEncodeA, int leftEncodeB, int rightEncodeA, int rightEncodeB);
        void getInfo(int info[]);
};


#endif