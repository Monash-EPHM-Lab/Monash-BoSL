/******************************************************************************
 DrainDetection Project
 Trial program for MS5803 pressure sensors with EC probes.
 Baiqian_Shi@Monash University
 12/02/2019

 *** series memory usage issue which may lead to faulty readings and SD card issue
 ******************************************************************************/

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <SoftwareSerial.h>
#include <JeeLib.h> // Low power functions library
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog
#include <avr/wdt.h>

SoftwareSerial Sim900(2,3);

//Declaring variables to store results
float EC, EC_Var, lastEC, Pressure, lastPressure, EC_sum, Pressure_sum;
float ECDiff, TempDiff, PressureDiff, FasterA;
short Counter, DailyCounter;
float Temp, lastTemp, Temp_sum, fasterA, ForceLog;
long batTimer, batInterval;
int Bat_Level;
int a, ScanInterval, MinLogInterval, MaxLogInterval, Writer;
String AAA, BBB;
unsigned long previous;
uint8_t x, answer;
char response[100];

//basic setup parameters battery level
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
float volt_Var, volt, volt_per;
// Begin class with selected address
// available addresses (selected by jumper on board) 
// default is ADDRESS_HIGH
// ADDRESS_HIGH = 0x76
// ADDRESS_LOW  = 0x77
MS5803 depth_sensor(ADDRESS_HIGH);

//Sensor pints
int ECInput = A1;
int ECPower = 7;

void setup() {
  // put your setup code here, to run once:
  ScanInterval = 10; //set the approximate seconds between each scan
  batInterval = 3600; //seconds between battey logging
  MinLogInterval = 60; //seconds
  MaxLogInterval = 3600; //seconds

  //this is for autologging function at SCAN interval! Just place a 100ohm 
  //shunt from 5v to A2 and it should enter into this logging
  pinMode(A2, INPUT); 

  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);

  //EC sensor
  pinMode(ECInput, INPUT);
  pinMode(ECPower, OUTPUT);
  digitalWrite(ECPower,LOW);

  //Serial communication
  Serial.begin(19200);
  Sim900.begin(19200);
  depth_sensor.begin();


  Temp_sum = 0.0;
  EC_sum = 0.0;
  Pressure_sum = 0.0;

  //go and get temperature and EC a couple of times to initiate things!
  GetSensorData();
  delay(1000);
  GetSensorData();

  //set placeholdernumbers
  //ForceLog = 1.01 * Pressure;
  lastEC = EC;
  lastTemp = Temp;
  lastPressure = Pressure;
  Temp_sum = Temp;
  EC_sum = EC;
  Pressure_sum = Pressure;

  //set Ec and temp
  SendToWeb222(1);

  Temp_sum = 0.0;
  EC_sum = 0.0;
  Pressure_sum = 0.0;
}

void loop() {
  // put your main code here, to run repeatedly:
  //turn off the TX and RX pins so the RX and TX pins of the SIM go low - saves power!
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  //get sensor of A5 to see if we need to do quick things
  FasterA = analogRead(A2)*1.0;

  //Get sensor data
  GetSensorData();
  //add these values to the averagers
  EC_sum += EC;
  Temp_sum += Temp;
  Pressure_sum += Pressure;

  //increment counters
  Counter += ScanInterval;
  DailyCounter += ScanInterval;
  batTimer += ScanInterval;
  
  //if we have gone for 60 seconds, record the data.
  if(Counter >= MinLogInterval || FasterA>900) {
    EC_sum = EC_sum / (Counter/ScanInterval);
    Temp_sum = Temp_sum / (Counter/ScanInterval);
    Pressure_sum = Pressure_sum / (Counter/ScanInterval);

    Serial.print(F(". It has been more than Min Log Interval. Previous average EC: "));
    Serial.print(lastEC);
    Serial.print(F(", Previous average Temp: "));
    Serial.print(lastTemp);
    Serial.print(F(", Previous average Pressure: "));
    Serial.print(lastPressure);
    
    Serial.print(F("Average EC: "));
    Serial.print(EC_sum);
    Serial.print(F(", Average Temp: "));
    Serial.print(Temp_sum);
    Serial.print(F(", Average Pressure: "));
    Serial.print(Pressure_sum);

    ECDiff = 0.00001;
    ECDiff = ((EC_sum*1.0-lastEC*1.0)/(lastEC*1.0+0.00001));
    ECDiff= (ECDiff*ECDiff);
    ECDiff = sqrt(ECDiff*1.0);
    TempDiff = 0.00001;
    TempDiff = ((Temp_sum*1.0-lastTemp*1.0)/(lastTemp*1.0+0.00001));
    TempDiff = (TempDiff*TempDiff);
    TempDiff = sqrt(TempDiff*1.0);
    PressureDiff = 0.00001;
    //PressureDiff = ((Pressure_sum*1.0-lastPressure*1.0)/(lastPressure*1.0+0.00001));
    PressureDiff = Pressure_sum*1.0-lastPressure*1.0;
    PressureDiff = (PressureDiff*PressureDiff);
    PressureDiff = sqrt(PressureDiff*1.0);

    Serial.print(F(", EC Diff: "));
    Serial.print(ECDiff);
    Serial.print(F(", Temp Diff: "));
    Serial.print(TempDiff);
    Serial.print(F(", Pressure Diff: "));
    Serial.print(PressureDiff);
    Serial.print(F(", DailyCounter: "));
    Serial.print(DailyCounter);
    Serial.print(F(", FasterA: "));
    Serial.println(FasterA);

    Writer = 0;

    if(ECDiff>=0.1){
      Writer = 1;
      Serial.println(F("We will write because ECDIFF was greater than 10%"));
    }
    if(TempDiff>=0.1){
      Writer = 1;
      Serial.println(F("We will write because TempDIFF was greater than 10%"));
    }
    if(PressureDiff>=1.0){
      Writer = 1;
      Serial.println(F("We will write because PressureDiff was greater than 1mbar"));
    }
    if(DailyCounter>=MaxLogInterval){
      Writer = 1;
      Serial.println(F("We will write because the time elapsed was greater than the max logging interval"));
    }
    //if(Pressure_sum>=ForceLog){
    //  Writer = 1;
    //  Serial.println(F("We will log because the current EC value was greater than the threshold set for minimum logging intervals"));
    //}    
    if(FasterA>=900.0){
      Writer = 1;
      Serial.println(F("We will log because there is a jumper between A2 and 5v"));
    }
      
    if(Writer==1){
      //ok we should send to the web because we have some changing datasets.
      //turn on the TX pin so the RX of the SIM goes high
      digitalWrite(3,HIGH);
      lastEC = EC_sum;
      lastTemp = Temp_sum;
      lastPressure = Pressure_sum;
      //also sends battery data if time has been long enough
      if (batTimer >= batInterval){
      batLev();
      SendToWeb222(2);
      batTimer = 0;
      }else{
      SendToWeb222(0);
      }
      
      Serial.print(F("Sent to web. "));
      //turn off the TX and RX pins so the RX and TX pins of the SIM go low - saves power!
      digitalWrite(3,LOW);
      DailyCounter = 0;
    } else{
      Serial.print(F("Skipped. "));
      //DailyCounter = 0;
    }
    //reset counters
    Counter = 0;
    EC_sum = 0;
    Temp_sum = 0;
    Pressure_sum = 0;
   }
  Serial.println(", delaying.");
  Sleepy::loseSomeTime(1000 * ScanInterval);
  //delay(1000 * ScanInterval);  
}

void SendToWeb222(int abc) {
    // abc = 0 is for normal send
    // abc = 1 initialises
    // abc = 2 sends battery data aswell
  a = 0;

  //first check to see if the modem is still on - if it is, turn it off
  while (sendATcommand(F("AT"),"OK",2000) == 1 && a < 5) {
   TurnOnOffSim();
   a++;
   delay(2000);
  } 
  
  //OK, it should now be off, now to turn it on
  a=0;
  while (sendATcommand(F("AT"),"OK",2000) == 0 && a<5) {
   TurnOnOffSim();
   a++;
   delay(2000);
  }
 
  if(abc==1){
      //FIRST TIME RUN ONLY!!!
      a=0;
      do{a++;}while (sendATcommand(F("AT+IPR=19200"),"OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("ATE0"),"OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("AT&W0"),"OK",10000) == 0 && a<5);
  }
  
  a=0;
  do{a++;}while (sendATcommand(F("AT+CGDCONT=1,\"IP\",\"mdata.net.au\",\"0.0.0.0\""), "OK",10000) == 0 && a<5);
  a=0;
  do{a++;}while (sendATcommand(F("AT+CGSOCKCONT=1,\"IP\",\"mdata.net.au\""), "OK",10000) == 0 && a<5);
  a=0;
  do{a++;}while (sendATcommand(F("AT+CSOCKSETPN=1"), "OK",10000) == 0 && a<10);
  a=0;
  do{a++;}while (sendATcommand(F("AT+CHTTPSSTART"), "OK",10000) == 0 && a<2);
  a=0;
  
  do{a++;}while (sendATcommand(F("AT+CHTTPSOPSE=\"www.cartridgerefills.com.au\",80,1"), "OK",10000) == 0 && a<5);
//you may need to move the /EoDC/ to the below line bewfore the "WriteMe.php... so it looks like "EoDC/WriteMe.php.... but try the way i have it first
  
  //normal send data
  if(abc == 0){
  AAA = "GET /";
  AAA += "EoDC/databases/WriteMe.php?SiteName=BI6384.csv&T=";
  AAA += Temp_sum;
  AAA += "&EC=";
  AAA += EC_sum;
  AAA += "&D=";
  AAA += Pressure_sum;
  /*//maybe needs
  AAA += "&BL=nul"
  */
  AAA += " HTTP/1.1\r\nHost: www.cartridgerefills.com.au:80\r\n\r\n";
  BBB = "AT+CHTTPSSEND=";
  BBB +=AAA.length();
  }
  //battery send data
  if(abc == 2){
  AAA = "GET /";
  AAA += "EoDC/databases/WriteMe.php?SiteName=BI6384.csv&T=";
  AAA += Temp_sum;
  AAA += "&EC=";
  AAA += EC_sum;
  AAA += "&D=";
  AAA += Pressure_sum;
  //these next 2 lines may need changing, not tested.
  AAA += "&BL=";
  AAA += volt_per;
  AAA += " HTTP/1.1\r\nHost: www.cartridgerefills.com.au:80\r\n\r\n";
  BBB = "AT+CHTTPSSEND=";
  BBB +=AAA.length();    
  }
  a=0;
  do{a++;}while (sendATcommand(BBB, ">",10000) == 0 && a<5);

  a=0;
  do{a++;}while (sendATcommand(AAA, "OK",10000) == 0 && a<5);

  delay(1000);

  a=0;
  do{a++;}while (sendATcommand(F("AT+CHTTPSCLSE"), "OK",5000) == 0 && a<1);

  a=0;
  do{a++;}while (sendATcommand(F("AT+CHTTPSSTOP"), "OK",10000) == 0 && a<1);

//  a=0;
//  do{a++;}while (sendATcommand(F("AT+CPOF"), "OK",10000) == 0 && a<1);

  //check to see if the modem is still on - if it is, turn it off
  while (sendATcommand(F("AT"),"OK",2000) == 1 && a < 5) {
   TurnOnOffSim();
   a++;
   delay(2000);
  } 
}

int8_t sendATcommand(String ATcommand, char* expected_answer1, unsigned int timeout){
    Serial.println(ATcommand);
    x=0;
    answer=0;
    
    previous = 0;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Sim900.available() > 0) Sim900.read();    // Clean the input buffer

    Sim900.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Sim900.available() != 0){    
            response[x] = Sim900.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    
    Serial.println(answer);
    return answer;
}



void TurnOnOffSim() {
   digitalWrite(8,LOW);
   delay(1000);
   digitalWrite(8,HIGH);
   delay(1200);
   digitalWrite(8,LOW);
   delay(5000); 
}

void GetSensorData() {
  //EC, Temp & Pressure
  //EC_Var = 0.0;
  
  //digitalWrite(ECPower, HIGH);
  delay(100);
  EC_Var = analogRead(ECInput);
  
  Temp = depth_sensor.getTemperature(CELSIUS, ADC_512);
  Pressure = depth_sensor.getPressure(ADC_4096)/10;
  //Depth = (100*(Pressure - Pressure_base))/(997*9.8)*1000;
  digitalWrite(ECPower, LOW);

  //EC Transformation
  //EC = EC_Var;
  EC = 0;
  //EC = map(EC_Var, 0, 1023, 0, 5000);
  //EC = (EC_Var*1025)/(5-EC_Var); //1K resistor
  //EC = EC_Var-25;
  //EC = (1000/(EC_Var*2.88));
  //EC = EC_Var / (1+ 0.019 *(Temp-25.0));

  //Print
  Serial.print(F("Recorded Temperature = "));
  Serial.print(Temp);
  Serial.print(F(", Recorded EC = "));
  Serial.print(EC);
  Serial.print(F(", Recorded Pressure = "));
  Serial.print(Pressure);
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