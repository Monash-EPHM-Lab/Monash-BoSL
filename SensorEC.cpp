#include <SensorEC.h>

SensorEC::SensorEC(uint8_t pinR, uint8_t pinA, uint8_t pinB = 0) : 
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

    storeLen = 0;
    ECSum = 0;
}


///////////////////////////////////////////////////////////////
void SensorEC::measure(void){ 
    uint16_t ECVar = 0;
    uint8_t reps = 2;

    for (uint8_t i = 0; i<reps; i++){
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

    return ECAverage;
}


///////////////////////////////////////////////////////////////
float SensorEC::getLast() {
    return lastEC;
}


///////////////////////////////////////////////////////////////
void SensorEC::clearSum(void) {
    lastEC = getAverage();  //save the average to last EC before clearing

    ECSum = 0;
    storeLen = 0;
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

    delay(100);//this value may need to be adjusted

    uint16_t readMax = 1023;
    uint16_t ECVal = analogRead(pinRead);
    if (polarity) {
        ECVal = readMax - ECVal;
    }

    Serial.print("EC = ");
    Serial.println(ECVal);

    if (pinVB){
        digitalWrite(pinVB,LOW);    
    }

    digitalWrite(pinVA,LOW);

    return ECVal;
}




