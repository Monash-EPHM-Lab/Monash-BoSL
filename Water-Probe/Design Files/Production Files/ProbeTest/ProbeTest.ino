/*
WIREING INSTRUCTIONS

HALF ORANGE  ---- No Connection
ORANGE       ---- 5V
HALF GREEN   ---- No Connection
BLUE         ---- SCL (A5)
HALF BLUE    ---- PIN 10
GREEN        ---- GND
HALF BROWN   ---- No Connection
BROWN        ---- SDA (A4)
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

#define ONE_WIRE_BUS 10

MS5803 presSense(ADDRESS_HIGH);
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);


void setup(void)
{

  Serial.begin(9600);
  Serial.println("#######BoSL SELF TEST#######");
    Serial.println("");
  sensors.begin();
  Wire.begin();
  presSense.reset();
  presSense.begin();
  
  //Check DS18B20
  for(int i =0; i < 5; i++){
      int temp;
      Serial.print("Requesting temperatures...");
      sensors.requestTemperatures();
      Serial.println("DONE");
     
      Serial.print("TEMP: ");
      
      temp = sensors.getTempCByIndex(0);
      Serial.println(temp);  
      if((-10 <temp) && (temp < 50)){
          Serial.println("###/// DS18B20 OKAY ///###");
          Serial.println(" ");
          break;
      }
      if (i == 4){
          Serial.println("###-----> DS18B20 FAIL <-----###");
          Serial.println(" ");
      }   
      
  }
  
  //Check MS5803
    if (presCheck()){
         Serial.println("###/// MS5803 OK ///###");
         Serial.println("");
     }else{
         Serial.println("###-----> MS5803 FAIL <-----###");
         Serial.println("");
     }
}


void loop(void)
{ 
}


bool presCheck(){
    
    presSense.reset();
    presSense.begin();

    int temp = presSense.getTemperature(CELSIUS, ADC_512);
    int pres = presSense.getPressure(ADC_4096);
    
    if (temp < 50 && temp > -10 && pres < 12000 && pres > 6000){
        return 1;
    }
    
    return(0);
}