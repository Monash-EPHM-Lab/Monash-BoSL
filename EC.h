#ifndef SensorEC_h
#define SensorEC_h

#include <Arduino.h>

class SensorEC
{
    public:
        SensorEC(uint8_t pinR, uint8_t pinA, uint8_t pinB = 0, const uint16_t Rdivider = 1225, const uint16_t Rcable = 250);
        //arguments: read pin, voltage pin A, voltage pin B, resistance divider, cable resistance.
        void measure(void);
        float getAverage(bool clear = false); // get the average EC value over the measurement interval
        void clearSum(void);  // clear and save the EC sum to last, beginning a new measurement 

        float getLast(void); //get the last EC value

    private:
        void pinSwitch(void);
        uint16_t read(void);
        float calEC(float ECAv);

        uint16_t ECSum;  // sum of EC measuremts, averaged on getEC call (each minute)
        uint8_t storeLen;  // number of EC values which have been measured and summed

        uint8_t pinVA; 
        uint8_t pinVB;
        const uint16_t Rd;
        const uint16_t Rc;
        const uint8_t pinRead;

        bool polarity = false;
        
        float lastEC;  //last EC value measured 
};

#endif
