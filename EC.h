#ifndef EC_h
#define EC_h

#include <Arduino.h>

 class EC
 {
     public:
        EC(bool, uint8_t, uint8_t, uint8_t);
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