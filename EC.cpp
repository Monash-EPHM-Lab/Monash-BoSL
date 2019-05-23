#include <EC.h>

EC::EC(uint8_t pinR, uint8_t pinA, uint8_t pinB, bool doSwitching = false) : 
    doSwitch(doSwitching),
    pinRead(pinR),
    pinVA(pinA),
    pinVB(pinB)
    {
    pinMode(pinRead,INPUT); //EC PIN is pinRead
    pinMode(pinVA,OUTPUT);//EC Power is pinVA
    pinMode(pinVB,OUTPUT);//EC Ground is pinVB
    digitalWrite(pinVB,LOW);//EC Ground is set to low 
    digitalWrite(pinVA,LOW);
    storeLen = 0;
}

 
void EC::measure(){ 
  uint16_t ECVar = 0;
  
  for (uint8_t i = 0; i<2; i++){
      digitalWrite(pinVB,LOW);
      digitalWrite(pinVA,HIGH);
      delay(100);//this value may need to be adjusted
      ECVar += ECread();
      digitalWrite(pinVB,LOW);
      digitalWrite(pinVA,LOW);
      pinSwitch();
  }
  ECVar = ECVar/2;

  ECVals[storeLen] = reading;
  storeLen++;
}

float getEC(clearVals = true) {
  uint16_t ECSum = 0;
  for (int i = 0; i<storeLen; i++) {
    ECSum += ECVals[i];
  }

  float ECAverage = (float)ECSum / (float)storeLen;

  return ECAverage
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
   uint16_t readMax = 1023;
   uint16_t ECVal = analogRead(pinRead);
   if (polarity) {
       ECVal = readMax - ECVal;
   }
   Serial.print("ECEC ");
   Serial.println(ECVal);
   return ECVal;
}



