#ifndef EC_h
#define EC_h

#include <Arduino.h>

 class EC
 {
     public:
        EC(uint8_t pinR, uint8_t pinA, uint8_t pinB, bool doSwitching = false);
        //arguments: read pin, voltage pin A, voltage pin B, do polarity switching.
        void measure(void);
        float getAverage(bool clear = true); // clear the EC averages by default
        void clear(void);
          
     private:
        void pinSwitch(void);
        uint16_t read(void);

        uint16_t ECSum;  // sum of EC measuremts, averaged on getEC call (each minute)
        uint8_t storeLen;  // number of EC values which have been measured and summed

        uint8_t pinVA; 
        uint8_t pinVB;
        const uint8_t pinRead;

        bool doSwitch;
        bool polarity = false;
};
 
 #endif
