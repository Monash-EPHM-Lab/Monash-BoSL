#include <EC.h>

/*
Application notes:
return value in tenths of an ohm

The attached EC probe should have the form.

VA------
        |
       | |  Rd
       | |  (Compoent resistor of about 100 Ohms)
        |
VR------
        |
       | |  Rc
       | | (Resistance of the measurement cable)
        |
       | |  Rl
       | | (Resistance of the liquid under test)
        |
VB------

VA, VB, VR are analogue pins with VA and VB on output and VR input.

From this the formula for calualting Rl is

Rl = Rd*((VA - VB - VR)/VR) - Rc

Typical values: 
(Note need to be calibrated for every sensor)
(Note these are not able to be measure with an ammeter suggested calibration involves adjusting values such that 0 is gotten on short and 3300 with a 330 ohm resistor)
(all values in tenths of an ohms)

Rd = 1225
Rc = 250
*/


SensorEC::SensorEC(uint8_t pinR, uint8_t pinA, uint8_t pinB = 0, const uint16_t Rdivider = 1225, const uint16_t Rcable = 250): 
	pinRead(pinR),
	pinVA(pinA),
	pinVB(pinB),
	Rc(Rcable),
	Rd(Rdivider),
	lastEC(0)
	{
	pinMode(pinRead,INPUT); //EC PIN is pinRead
	pinMode(pinVA,OUTPUT);//EC Power is pinVA
	digitalWrite(pinVA,LOW);

    if (pinVB) { // if pinB is not 0 (anything else) then activate switching mode
		pinMode(pinVB,OUTPUT);//EC Ground is pinVB
		digitalWrite(pinVB,LOW);//EC Ground is set to low       
  	}

  	storeLen = 0;
  	ECSum = 0;
}


///////////////////////////////////////////////////////////////
void SensorEC::measure(void){ 
	uint16_t ECVar = 0;
	uint8_t reps = 2;

	for (uint8_t i = 0; i<reps; i++) {
		ECVar += read();
		pinSwitch();
	}

	ECSum += ECVar; // add measurement to total sum
	storeLen += reps;  // add reps for number of measurements taken
}


///////////////////////////////////////////////////////////////
float SensorEC::getAverage(bool clear = false) {
	float ECAverage = (float)ECSum / (float)storeLen;
  

	if (clear) {
		clearSum();
	}

  	float ECAverageCal = calEC(ECAverage); 

  	return ECAverageCal;
}


///////////////////////////////////////////////////////////////
void SensorEC::clearSum(void) {
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


///////////////////////////////////////////////////////////////
uint16_t SensorEC::read(){
  	digitalWrite(pinVA,HIGH);

    if (pinVB) {
        digitalWrite(pinVB,LOW);
    }

	delay(100); //this value may need to be adjusted

	uint16_t readMax = 1023;
	uint16_t ECVal = analogRead(pinRead);
  	if (polarity) {
    	ECVal = readMax - ECVal;
 	}

  	if (pinVB){
    	digitalWrite(pinVB,LOW);    
  	}

  	digitalWrite(pinVA,LOW);
  
  	Serial.print("EC = ");
  	Serial.println(ECVal);
  
  	return ECVal;
}


///////////////////////////////////////////////////////////////
float SensorEC::calEC(float ECAv) {  // calibrate the raw analog reading to a resistance
    float divmath = (float)ECAv / ((float)(1024-ECAv));

    return ((((Rd)*divmath) - (Rc))/10); 
}