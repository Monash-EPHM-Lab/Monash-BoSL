#ifndef SensorEC_h
#define SensorEC_h

#include <Arduino.h>

class Sensor
{
    public:
        void measure(void);
        float getAverage(bool clear = false); // get the average EC value over the measurement interval
        void clearAndSave(void);  // clear and save the EC sum to last, beginning a new measurement 

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
