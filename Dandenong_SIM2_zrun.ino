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
#include <JeeLib.h> // Low power functions library
#include <Battery.h>
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog
#include <avr/wdt.h>


//Declaring variables to store results
float EC, EC_Var, lastEC, Pressure, lastPressure, EC_sum, Pressure_sum;
float ECDiff, TempDiff, PressureDiff, FasterA;
short Counter, DailyCounter;
float Temp, lastTemp, Temp_sum, fasterA, ForceLog;
int a, ScanInterval, MinLogInterval, MaxLogInterval, Writer;
String AAA, BBB;
unsigned long previous;
uint8_t x, answer;
char response[100];


//Battery level reader
//Variables
float volt_Var, volt, volt_per;
//basic setup parameters
int RGI = 1000; //voltage devider, resistance between ground and input pin
int RIV = 1000; //voltage devider, resistance between input pin and vin
int BatPin = A3; //input pin
int lowVolt = 7200; //battery cutoff voltage
int highVolt = 8400; //battery full charge voltage
int vinReduce = 780; //calibration factor between battery voltage and vin voltage --no need to adjust
//end basic setup parametes
int rdLow = lowVolt - vinReduce;
int rdHigh = highVolt - vinReduce;
//decoment the formula once the values for RGI and RIV are known.
float divRatio = 2;//(RGI+RIV)/RGI;
float pinValCal = float(5000)/float(1023); //1023 and 5030 are pin value and voltage at maximum

// Begin class with selected address
// available addresses (selected by jumper on board) 
// default is ADDRESS_HIGH
//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77
MS5803 depth_sensor(ADDRESS_HIGH);

//Sensor pints
int ECInput = A1;
int ECPower = 7;

//MOSFET pins
int MOS_G = 6;

SoftwareSerial Sim900(2,3);


void setup() {
  //EC sensor
  pinMode(ECInput, INPUT);
  pinMode(ECPower, OUTPUT);
  digitalWrite(ECPower,LOW);

  //MOSFET pin
  pinMode(MOS_G, OUTPUT);
  digitalWrite(MOS_G,LOW);

  Serial.begin(9600);
}


void loop() {
  //digitalWrite
  digitalWrite(MOS_G,HIGH);
  batLev();
  digitalWrite(MOS_G,LOW);
  Serial.println("pin_6: HIGH");
  Serial.print("Battery Level: ");
  Serial.print(volt/1000);
  //Serial.print(volt_Var);
  Serial.println("V");
  Serial.print("Remain Percentage: ");
  Serial.print(volt_per);
  Serial.println("%");
  //Serial.print("Flow Rate: ");
  //Serial.println(methsn.flowSample());
  delay(1000);
  batLev();
  Serial.println("pin_6: LOW");
  Serial.print("Battery Level: ");
  Serial.print(volt/1000);
  //Serial.print(volt_Var);
  Serial.println("V");
  Serial.print("Remain Percentage: ");
  Serial.print(volt_per);
  Serial.println("%");
  delay(1000);
}

void batLev() {
  volt_Var = analogRead(BatPin);
  volt = map(volt_Var, 0, 1023, 0, 5000);
  volt = (volt * divRatio + vinReduce);
  volt_per = (100*(float)(volt - lowVolt)/(highVolt - lowVolt));
  if (volt_per < 0){
     volt_per = 0;
  }
}
