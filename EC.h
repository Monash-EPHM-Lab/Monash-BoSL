#ifndef EC_h
#define EC_h

#include <Arduino.h>

 class EC
 {
     public:
        EC(uint8_t pinR, uint8_t pinA, uint8_t pinB, bool doSwitching = true);
        //arguments: read pin, voltage pin A, voltage pin B, do polarity switching.
        uint16_t measure(void);
        void pinSwitch(void);
        
        
     private:
        bool doSwitch;
        const uint8_t pinVA; 
        const uint8_t pinVB;
        uint8_t pinVAswitch; 
        uint8_t pinVBswitch;
        uint8_t pinSwitcher;
        const uint8_t pinRead;
        bool polarity = false;
        uint16_t ECread(void);

};
 
 #endif