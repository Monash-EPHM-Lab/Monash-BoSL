#include "SensorEC.h"

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

//500ms delay values
//#define CAL_M -0.0724 
//#define CAL_C 70.9  

#define EC_DELAY 1 //in ms
#define CAL_M -0.0568 //calibration gradient for 0.1-15 ms/cm range. CALIBRATED IN LAB 22/07/19
#define CAL_C 54.0    // calibration constant. CALIBRATED IN LAB 22/07/19
// calibrated for 100ms delay


SensorEC::SensorEC(uint8_t pinR, uint8_t pinA, uint8_t pinB) : 
	pinRead(pinR),
	pinVA(pinA),
	pinVB(pinB)
	{
	pinMode(pinRead,INPUT); //EC PIN is pinRead
	pinMode(pinVA,OUTPUT);//EC Power is pinVA
	digitalWrite(pinVA,LOW);

  if (pinVB) { // if pinB is not 0 (anything else) then activate switching mode
  	pinMode(pinVB,OUTPUT);//EC Ground is pinVB
  	digitalWrite(pinVB,LOW);//EC Ground is set to low       
	}
}


///////////////////////////////////////////////////////////////
uint16_t SensorEC::measure(void){ 
  uint16_t ECVar = 0;
  uint8_t reps = 2;

  for (uint8_t i = 0; i<reps; i++) {
    ECVar += read();
    pinSwitch();
  }

  return ECVar / reps;
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

  delay(EC_DELAY); //this value may need to be adjusted
    // the delay time is very important. increasing delay to 500ms can increase analog readings
    // by 8% in some circumstances.

  uint16_t readMax = 1023;
  uint16_t ECVal = analogRead(pinRead);
  ECVal = analogRead(pinRead);

  if (polarity) {
    ECVal = readMax - ECVal;
  }

  if (pinVB){
    digitalWrite(pinVB,LOW);    
  }

  digitalWrite(pinVA,LOW);

  return ECVal;
}

///////////////////////////////////////////////////////////////
float SensorEC::calEC(float ECAv) {  // calibrate the raw analog reading to a resistance

// THESE VALUES ARE BASED ON LAB CALIBRATION CURVE

    return CAL_M * ECAv + CAL_C;
}
