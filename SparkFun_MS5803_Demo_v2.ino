
/******************************************************************************
SparkFun_MS5803_Demo.ino
Demo Program for MS5803 pressure sensors.
Casey Kuhns @ SparkFun Electronics
7/20/2014
https://github.com/sparkfun/MS5803-14BA_Breakout/

The MS58XX MS57XX and MS56XX by Measurement Specialties is a low cost I2C pressure
sensor.  This sensor can be used in weather stations and for altitude
estimations. It can also be used underwater for water depth measurements. 

Resources:
This library uses the Arduino Wire.h to complete I2C transactions.

Development environment specifics:
	IDE: Arduino 1.0.5
	Hardware Platform: Arduino Pro 3.3V/8MHz
	T5403 Breakout Version: 1.0

**Updated for Arduino 1.6.4 5/2015**
	
This code is beerware. If you see me (or any other SparkFun employee) at the
local pub, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

// Begin class with selected address
// available addresses (selected by jumper on board) 
// default is ADDRESS_HIGH

//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77

MS5803 sensor(ADDRESS_HIGH);

//Create variables to store results
double EC, EC_Var;
double Pressure, Pressure_base, Depth, counter;
float Temp;

//Sensor pints
int ECInput = A1;
int ECPower = 7;

// Create Variable to store altitude in (m) for calculations;
// double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)

void setup() {
    Serial.begin(9600);
    //Retrieve calibration constants for conversion math.
    sensor.reset();
    sensor.begin();
    pinMode(ECInput, INPUT);
    pinMode(ECPower, OUTPUT);
    digitalWrite(ECPower,LOW);
    
    Pressure_base = sensor.getPressure(ADC_2048)/10;
}

void loop() {
  counter = counter + 1;
  // To measure to higher degrees of precision use the following sensor settings:
  // ADC_256 
  // ADC_512 
  // ADC_1024
  // ADC_2048
  // ADC_4096
  EC_Var = 0.0;
  digitalWrite(ECPower, HIGH);
  delay(100);
  EC_Var = analogRead(ECInput);
  delay(100);  
  EC_Var = analogRead(ECInput);
  
  Temp = sensor.getTemperature(CELSIUS, ADC_512);
  Pressure = sensor.getPressure(ADC_4096)/10;
  Depth = (100*(Pressure - Pressure_base))/(997*9.8)*1000;
 // digitalWrite(ECPower, LOW);

  //EC Transformation
  //EC = map(EC_Var, 0, 1023, 0, 5000);
  //EC = (EC_Var*1025)/(5-EC_Var); //1K resistor
  //EC = EC_Var-25;
  //EC = (1000/(EC_Var*2.88));
  //EC = EC_Var / (1+ 0.019 *(Temp-25.0));

  Serial.print(counter);
  Serial.print(",");
  Serial.print(EC_Var);
  Serial.print(",");
  Serial.print(Temp);
  Serial.print(",");
  Serial.print(Pressure_base);
  Serial.print(",");
  Serial.print(Pressure);
  Serial.print(",");
  Serial.println(Depth);
  
  delay(1000);

  }
