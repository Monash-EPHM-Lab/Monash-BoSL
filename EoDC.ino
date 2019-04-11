#include "EoDCLib/BatLevel.h"
#include "EoDCLib/SensorData.h"
#include "EoDCLib/Sleep.h"



void setup(){
    const uint8_t x = 3;
    const uint8_t y = 5;
    Serial.begin(9600);
    SensorData data = SensorData(x,y);
    
  //  Serial.println(data.getVal());
}
void loop(){}
