
#include <JeeLib.h>
ISR(WDT_vect) { Sleepy::watchdogEvent(); }


void setup(){



}


void loop(){

   // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
   Sleepy::loseSomeTime(10000);
    delay(10000);
}