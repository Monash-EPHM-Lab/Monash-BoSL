#include "EoDCLib/BatLevel.h"
#include "EoDCLib/SensorData.h"
#include "EoDCLib/Sleep.h"



void setup(){
  
    Serial.begin(9600);
    data = SensorData(3,5);
    
    Serial.println(data.getVal());
}
void loop(){}
