#include <EC.h>

EC::EC(bool doSwitching, uint8_t pinR, uint8_t pinA, uint8_t pinB) : 
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

    pinVAswitch = pinVA;
    pinVBswitch = pinVB;
}

 
uint16_t EC::measure(){ // make sure shift works properly
  uint16_t ECVar = 0;
  
  digitalWrite(pinVBswitch,LOW);
  digitalWrite(pinVAswitch,HIGH);
  delay(100);//this value may need to be adjusted
  ECVar = ECread();
  digitalWrite(pinVBswitch,LOW);
  digitalWrite(pinVAswitch,LOW);
  pinSwitch();
  return(ECVar);
}

void EC::pinSwitch(){
   if (doSwitch == true){
   pinSwitcher = pinVAswitch;
   pinVAswitch = pinVBswitch;
   pinVBswitch = pinSwitcher;
   polarity = !polarity;
   }
}

uint16_t EC::ECread(){
   uint16_t readMax = 1012;
   uint16_t ECVal = analogRead(pinRead);
   if (polarity == true){
       ECVal = readMax - ECVal;
   }
   return ECVal;
}