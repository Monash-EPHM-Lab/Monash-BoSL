//FOR USE WITH 8 MHz ATMEGA328P//
#ifndef Sleeps_h
#define Sleeps_h

#include <Arduino.h>

class SleepClass
{
    public:
        void extDelay(uint32_t);
        void sleeps(uint16_t);
    private:
        void timer2on(void);
};

#endif

extern SleepClass Sleeps;

