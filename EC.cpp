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


EC::EC(uint8_t pinR, uint8_t pinA, uint8_t pinB = -1, bool doSwitching = false, const uint16_t Rdevider = 1225, const uint16_t Rcable = 250): 
  doSwitch(doSwitching),
  pinRead(pinR),
  pinVA(pinA),
  pinVB(pinB),
  Rc(Rcable),
  Rd(Rdevider)
  {
  pinMode(pinRead,INPUT); //EC PIN is pinRead
  pinMode(pinVA,OUTPUT);//EC Power is pinVA
  digitalWrite(pinVA,LOW);

  if (doSwitch) { // -1 is default value for unspecfied pinB. must specify for switching.
    pinMode(pinVB,OUTPUT);//EC Ground is pinVB
    digitalWrite(pinVB,LOW);//EC Ground is set to low       
  }

  storeLen = 0;
  ECSum = 0;
}

 
void EC::measure(void){ 
  uint16_t ECVar = 0;
  uint8_t reps = 2;

  for (uint8_t i = 0; i<reps; i++){
      ECVar += read();
      pinSwitch();
  }

  //Serial.print("EC = ");
  //Serial.println(ECVar/reps);

  ECSum += ECVar; // add measurement to total sum
  storeLen += reps;  // add reps for number of measurements taken
}

float EC::getAverage(bool clear = true) {
  float ECAverage = (float)ECSum / (float)storeLen;
  
  
  if (clear) {
    clearSum();
  }
  float ECAverageCal = ECCal(ECAverage); 

  return ECAverageCal;
}


void EC::clearSum(void) {
  ECSum = 0;
  storeLen = 0;
}


void EC::pinSwitch(){
  if (doSwitch) {
    uint8_t temp = pinVA;
    pinVA = pinVB;
    pinVB = temp;
    polarity = !polarity;
  }
}


uint16_t EC::read(){
  digitalWrite(pinVA,HIGH);

  if (doSwitch) {
    digitalWrite(pinVB,LOW);
  }

  //delay(100);
  delay(3000);//this value may need to be adjusted
  uint16_t readMax = 1023;
  uint16_t ECVal = analogRead(pinRead);
  if (polarity) {
    ECVal = readMax - ECVal;
  }

  if (doSwitch){
    digitalWrite(pinVB,LOW);    
  }
  digitalWrite(pinVA,LOW);
  
  Serial.println(ECVal);
  
  return ECVal;
}

float EC::ECCal(float ECAv){
   
    float divmath = (float)ECAv/((float)(1024-ECAv));

    return ((((Rd)*divmath) - (Rc))/10); 

}