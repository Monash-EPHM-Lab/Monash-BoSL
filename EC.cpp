#include <EC.h>

EC::EC(uint8_t pinR, uint8_t pinA, uint8_t pinB = -1, bool doSwitching = false) : 
    doSwitch(doSwitching),
    pinRead(pinR),
    pinVA(pinA),
    pinVB(pinB)
    {
    pinMode(pinRead,INPUT); //EC PIN is pinRead

    pinMode(pinVA,OUTPUT);//EC Power is pinVA
    digitalWrite(pinVA,LOW);

    if (doSwitching) { // -1 is default value for unspecfied pinB. must specify for switching.
      pinMode(pinVB,OUTPUT);//EC Ground is pinVB
      digitalWrite(pinVB,LOW);//EC Ground is set to low       
    }
    storeLen = 0;
    ECSum = 0;
}

 
void EC::measure(){ 
  uint16_t ECVar = 0;
  uint8_t reps = 2;

  for (uint8_t i = 0; i<reps; i++){
      ECVar += ECread();
      pinSwitch();
  }

  ECSum += ECVar; // add measurement to total sum
  storeLen += reps;  // add reps for number of measurements taken
}


float getEC() {
  float ECAverage = (float)ECSum / (float)storeLen;

  storeLen = 0;
  ECSum = 0;

  return ECAverage;
}


void EC::pinSwitch(){
  if (doSwitch) {
    uint8_t temp = pinVA;
    pinVA = pinVB;
    pinVB = temp;
    polarity = !polarity;
  }
}


uint16_t EC::ECread(){
  digitalWrite(pinVA,HIGH);
  if (doSwitching) {
    digitalWrite(pinVB,LOW);
  }

  delay(100);//this value may need to be adjusted

  uint16_t readMax = 1023;
  uint16_t ECVal = analogRead(pinRead);
  if (polarity) {
    ECVal = readMax - ECVal;
  }

  Serial.print("EC = ");
  Serial.println(ECVal);

  if (doSwitching){
    digitalWrite(pinVB,LOW);    
  }
  digitalWrite(pinVA,LOW);
  
  return ECVal;
}



