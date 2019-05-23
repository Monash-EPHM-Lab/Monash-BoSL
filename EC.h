#ifndef EC_h
#define EC_h

#include <Arduino.h>

 class EC
 {
     public:
        EC(uint8_t pinR, uint8_t pinA, uint8_t pinB, bool doSwitching = false);
        //arguments: read pin, voltage pin A, voltage pin B, do polarity switching.
        uint16_t measure(void);
          
     private:
        void pinSwitch(void);
        uint16_t ECread(void);
        float getEC(clearVals = true;);

        uint16_t ECVals[10];  // array to store multiple EC measuremts, averaged on getValue call (each minute)
        uint8_t storeLen;

        uint8_t pinVA; 
        uint8_t pinVB;
        const uint8_t pinRead;

        bool doSwitch;
        bool polarity = false;
};
 
 #endif