#ifndef BatLevel_h
#define BatLevel_h

#include <Arduino.h>

 class BatLevel
 {
     private:
        //basic setup parameters
        int RGI;// = 10; //voltage devider, resistance between ground and input pin
        int RIV;// =  660; //voltage devider, resistance between input pin and vin
        int BatPin;// = A2; //input pin
        int lowVolt; //= 7200; //battery cutoff voltage
        int highVolt;// = 8400; //battery full charge voltage
        int vinReduce;// = 780; //calibration factor between battery voltage and vin voltage --no need to adjust
        //end basic setup parametes
        
        
        int rdLow;// = lowVolt - vinReduce;
        int rdHigh;// = highVolt - vinReduce;
        
        //decoment the formula once the values for RGI and RIV are known.
        float divRatio;// = 1.663;//(RGI+RIV)/RGI;
        float pinValCal;// = float(5030)/float(1023); //1023 and 5030 are pin value and voltage at maximum
        
     public:
        BatLevel(void);
        int readVal(void);//returns unprocessed pin reading of battery
        int readVolt(void); //returns battery voltage in mV
        int readLev(void); //returns estimated battery level in %
};
 
 #endif