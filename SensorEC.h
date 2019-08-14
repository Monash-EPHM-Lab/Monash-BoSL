#ifndef SensorEC_h
#define SensorEC_h

#include <Arduino.h>

class SensorEC
{
    public:
        SensorEC(uint8_t pinR, uint8_t pinA, uint8_t pinB = 0);
        //arguments: read pin, voltage pin A, voltage pin B.

        uint16_t measure(void);  // return measured ADC value
        
    private:
        int16_t read(void);
        void pinSwitch(void);
        float calEC(float ECAv);

        uint8_t pinVA; 
        uint8_t pinVB;
        const uint8_t pinRead;

        bool polarity = false;
};

#endif
