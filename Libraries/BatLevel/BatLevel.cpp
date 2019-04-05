#include <BatLevel.h>

BatLevel::BatLevel(void):

    //basic setup parameters
    RGI(10),// = 10; //voltage devider, resistance between ground and input pin
    RIV(10),// =  660; //voltage devider, resistance between input pin and vin
    BatPin(A0),// = A2; //input pin
    lowVolt(7200), //= 7200; //battery cutoff voltage
    highVolt(8400),// = 8400; //battery full charge voltage
    vinReduce(780),// = 780; //calibration factor between battery voltage and vin voltage --no need to adjust
    //end basic setup parametes
    
    
    rdLow(lowVolt - vinReduce),// = lowVolt - vinReduce;
    rdHigh(highVolt - vinReduce),// = highVolt - vinReduce;
    
    //decoment the formula once the values for RGI and RIV are known.
    divRatio(2)// = 1.663;//(RGI+RIV)/RGI;
    {
        
}

int BatLevel::readVal (){ //returns unprocessed pin reading of battery
            return  analogRead(BatPin);
        }
        
int BatLevel::readVolt (void){ //returns battery voltage in mV
            int rdVal = readVal();
            int volt = (int)map(rdVal,0,1023,0,5000)*divRatio + vinReduce;  
            return volt;
        }

int BatLevel::readLev (void){ //returns estimated battery level in %
            int rdvolt = readVolt();
            int batLev  = (100*(float)(rdvolt - lowVolt)/(highVolt - lowVolt));
            if (batLev < 0){
                    return 0;
            }
            else{
                return batLev;
            }
        }