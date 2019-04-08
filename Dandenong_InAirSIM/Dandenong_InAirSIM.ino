/******************************************************************************
 DrainDetection Project
 Measuring the atomspheric pressure in air.
 Baiqian_Shi@Monash University
 07/03/2019

 *** series memory usage issue which may lead to faulty readings and SD card issue
 ******************************************************************************/

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <SoftwareSerial.h>
#include <BatLevel.h>
#include <JeeLib.h> // Low power functions library
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog
#include <avr/wdt.h>

SoftwareSerial Sim900(2,3);

//Declaring variables to store results
float EC, Pressure, Pressure_sum, Temp, Temp_sum, FasterA, batLev;
short Counter;
int a, ScanInterval, LogInterval, Writer;
String AAA, BBB;
String SiteID = "developement";
unsigned long previous;
uint8_t x, answer;
char response[100];

// Begin class with selected address
// available addresses (selected by jumper on board) 
// default is ADDRESS_HIGH
//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77
MS5803 depth_sensor(ADDRESS_HIGH);

//create battery object:
BatLevel batlv;

void setup() {
  // put your setup code here, to run once:
  ScanInterval = 60; //set the approximate seconds between each scan
  LogInterval = 360; //seconds

  //this is for autologging function at SCAN interval! Just place a 100ohm 
  //shunt from 5v to A2 and it should enter into this logging
  pinMode(A2, INPUT); 

  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);

  //Serial communication
  Serial.begin(19200);
  Sim900.begin(19200);
  depth_sensor.begin();

  Temp_sum = 0.0;
  EC = 0.0;
  Pressure_sum = 0.0;

  //go and get temperature and EC a couple of times to initiate things!
  GetSensorData();
  delay(1000);
  GetSensorData();

  //set placeholdernumbers
  Temp_sum = Temp;
  Pressure_sum = Pressure;

  //set Ec and temp
  SendToWeb222(1);

  Temp_sum = 0.0;
  EC = 0.0;
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
  Temp_sum = Temp_sum + Temp;
  Pressure_sum = Pressure_sum + Pressure;

  //increment counters
  Counter = Counter + ScanInterval;

  //if we have gone for 60 seconds, record the data.
  if(Counter >= LogInterval || FasterA>900) {
    EC = 0.0;
    Temp_sum = Temp_sum / (Counter/ScanInterval);
    Pressure_sum = Pressure_sum / (Counter/ScanInterval);
    Writer = 1;

    Serial.print(F(". It has been more than the Log Interval. Average EC: "));
    Serial.print(EC);
    Serial.print(F(", Average Temp: "));
    Serial.print(Temp_sum);
    Serial.print(F(", Average Pressure: "));
    Serial.print(Pressure_sum);
    Serial.print(F(", FasterA: "));
    Serial.println(FasterA);

    if(Writer==1){
      //ok we should send to the web because we have some changing datasets.
      //turn on the TX pin so the RX of the SIM goes high
      digitalWrite(3,HIGH);
      SendToWeb222(0);
      Serial.print(F("Sent to web. "));
      //turn off the TX and RX pins so the RX and TX pins of the SIM go low - saves power!
      digitalWrite(3,LOW);
    } else{
      Serial.print(F("Skipped. "));
    }
    //resent counters
    Counter = 0;
    EC = 0;
    Temp_sum = 0;
    Pressure_sum = 0;
  }
  Serial.println(", delaying.");
  Sleepy::loseSomeTime(9800 * ScanInterval);
  //delay(1000 * ScanInterval);  
}

void SendToWeb222(int abc) {
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
  AAA = "GET /";
  AAA += "EoDC/databases/WriteMe.php?SiteName="+SiteID+".csv&T=";
  AAA += Temp_sum;
  AAA += "&EC=";
  AAA += EC;
  AAA += "&D=";
  AAA += Pressure_sum;
  AAA += "&B=";
  AAA += batLev;
  AAA += " HTTP/1.1\r\nHost: www.cartridgerefills.com.au:80\r\n\r\n";
  BBB = "AT+CHTTPSSEND=";
  BBB +=AAA.length();

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
  batLev = batlv.readLev();
  Temp = depth_sensor.getTemperature(CELSIUS, ADC_512);
  Pressure = depth_sensor.getPressure(ADC_4096)/10;
  //Depth = (100*(Pressure - Pressure_base))/(997*9.8)*1000;

  //Print
  Serial.print(F("Recorded Temperature = "));
  Serial.print(Temp);
  Serial.print(F(", Recorded EC = "));
  Serial.print(EC);
  Serial.print(F(", Recorded Pressure = "));
  Serial.print(Pressure);
}
