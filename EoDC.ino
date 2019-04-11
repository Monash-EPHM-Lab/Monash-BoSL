#include "BatLevel.h"

   
   BatLevel batlv;

void setup(){
    
  Serial.begin(19200);
}

void loop(){
    
    Serial.println(batlv.readVolt());
    delay(1000);
    
    
}