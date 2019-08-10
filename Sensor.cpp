#include "Sensor.h"

/*
Base Sensor class from which temperature / EC/ pressure are derived.
*/




///////////////////////////////////////////////////////////////
void Sensor::measure(void){ 
	int16_t var = read();

	sum += var; // add measurement to total sum
	storeLen ++;  // add 1 for number of measurements taken
}


///////////////////////////////////////////////////////////////
float SensorEC::getAverage(bool clear = false) {
	float ECAverage = (float)ECSum / (float)storeLen;
  

	if (clear) {
		clearAndSave();
	}

  	//ECAverage = calEC(ECAverage);  
    // do not calibrate at the moment. return raw ADC values

  return ECAverage;
}


///////////////////////////////////////////////////////////////
void SensorEC::clearAndSave(void) {
    lastEC = getAverage();  //save the average to last EC before clearing

	  ECSum = 0;
    storeLen = 0;
}


///////////////////////////////////////////////////////////////
float SensorEC::getLast() {
    return lastEC;
}


///////////////////////////////////////////////////////////////
void SensorEC::pinSwitch(){
    if (pinVB) { //if pinVB is defined (not 0) then switch
        uint8_t temp = pinVA;
        pinVA = pinVB;
        pinVB = temp;
        polarity = !polarity;
    }
}





