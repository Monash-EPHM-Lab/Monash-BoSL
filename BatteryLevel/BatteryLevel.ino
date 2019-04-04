/******************************************************************************
 DrainDetection Project
 Baiqian_Shi@Monash University
 
 Trial program for MS5803 pressure sensors with EC probes.
 
 File Author: Bainqian Shi
 
 12/02/2019
 *** series memory usage issue which may lead to faulty readings and SD card issue
 ******************************************************************************/

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <SoftwareSerial.h>

#include <BatLevel.h>



//create battery object
BatLevel batlv;

void setup() {

  Serial.begin(19200);

}

void loop() {
 
  Serial.println(batlv.readVolt());
  delay(1000);
}

