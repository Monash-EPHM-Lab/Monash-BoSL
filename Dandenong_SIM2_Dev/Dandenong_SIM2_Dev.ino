/******************************************************************************
 DrainDetection Project
 Trial program for MS5803 pressure sensors with EC probes.
 Baiqian_Shi@Monash University
 12/02/2019

 *** series memory usage issue which may lead to faulty readings and SD card issue
 ******************************************************************************/
//setup quick scan and record function from website...so it reads a file and then enters a quick scan and upload sequence until that file is changed again.

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <SoftwareSerial.h>
#include <JeeLib.h> // Low power functions library
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog
#include <avr/wdt.h>

SoftwareSerial Sim900(2,3);

float temp, EC, Depth, TempVar, ECVar, DepthVar, lastECwritten, lastTempwritten, lastDepthwritten, ECDiff, TempDiff, DepthDiff, FasterA, ECThresholdForForcedLoggingAtMinInterval;
short counter, DailyCounter;
int a, ScanInterval, MinLogInterval, MaxLogInterval, Writer;
String AAA, BBB;
unsigned long previous;
uint8_t x, answer;
char response[100];
bool polarityEC;



// Begin class with selected address
// available addresses (selected by jumper on board) 
// default is ADDRESS_HIGH
//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77
MS5803 depth_sensor(ADDRESS_HIGH);

void setup() {
  ScanInterval = 10; //seconds
  MinLogInterval = 60; //seconds
  MaxLogInterval = 3600; //seconds
  ECThresholdForForcedLoggingAtMinInterval = 0.8;
  pinMode(A2,INPUT); //this is for autologging function at SCAN interval! Just place a 100ohm shunt from 5v to A5 and it should enter into this logging
  
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);

  //Probe stuff
  pinMode(A0,INPUT); //EC PIN is A0
  pinMode(A3,OUTPUT);//EC Power is A3
  pinMode(A1,OUTPUT);//EC Ground is A1
  digitalWrite(A1,LOW);//EC Ground is set to low 
  digitalWrite(A3,LOW);

  Serial.begin(19200);
  Sim900.begin(19200);
  depth_sensor.begin();
  
  temp = 0.0;
  EC = 0.0;
  Depth = 0.0;
  polarityEC = 0;
  
  //go and get temperature and EC a couple of times to initiate things!
  GetTempANDEC();
  delay(1000);
  GetTempANDEC();

  //set placeholdernumbers
  lastECwritten = ECVar;
  lastTempwritten = TempVar;
  lastDepthwritten = DepthVar;
  EC = ECVar;
  temp = TempVar;
  Depth = DepthVar;

  //set Ec and temp
  SendToWeb222(1);

  EC = 0.0;
  temp = 0.0;
  Depth = 0.0;
}

void loop() {
  //turn off the TX and RX pins so the RX and TX pins of the SIM go low - saves power!
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);

  //get sensor of A5 to see if we need to do quick things
  FasterA = analogRead(A2)*1.0;

  //go and get temperature and EC
  GetTempANDEC();

  //add these values to the averagers
  EC = EC + ECVar;
  temp = temp + TempVar;
  Depth = Depth + DepthVar;

  //increment counters
  counter = counter + ScanInterval;
  DailyCounter = DailyCounter + ScanInterval;

  //if we have gone for 60 seconds, record the data.
  if(counter >= MinLogInterval || FasterA>900) {
    EC = EC / (counter/ScanInterval);
    temp = temp / (counter/ScanInterval);
    Depth = Depth / (counter/ScanInterval);

    Serial.print(F(". It has been more than Min Log Interval. Previous average EC: "));
    Serial.print(lastECwritten);
    Serial.print(F(", Previous average Temp: "));
    Serial.print(lastTempwritten);
    
    Serial.print(F("Average EC: "));
    Serial.print(EC);
    Serial.print(F(", Average Temp: "));
    Serial.print(temp);
    Serial.print(F(", Average Depth: "));
    Serial.print(Depth);

    ECDiff = 0.00001;
    ECDiff = ((EC*1.0-lastECwritten*1.0)/(lastECwritten*1.0+0.00001));
    ECDiff= (ECDiff*ECDiff);
    ECDiff = sqrt(ECDiff*1.0);
    TempDiff = 0.00001;
    TempDiff = ((temp*1.0-lastTempwritten*1.0)/(lastTempwritten*1.0+0.00001));
    TempDiff = (TempDiff*TempDiff);
    TempDiff = sqrt(TempDiff*1.0);
    DepthDiff = 0.00001;
    //DepthDiff = ((Depth*1.0-lastDepthwritten*1.0)/(lastDepthwritten*1.0+0.00001));
    DepthDiff = Depth*1.0 - lastDepthwritten*1.0;
    DepthDiff = (DepthDiff*DepthDiff);
    DepthDiff = sqrt(DepthDiff*1.0);
    
    Serial.print(F(", EC Diff: "));
    Serial.print(ECDiff);
    Serial.print(F(", Temp Diff: "));
    Serial.print(TempDiff);
    Serial.print(F(", Depth Diff: "));
    Serial.print(DepthDiff);
    Serial.print(F(", DailyCounter: "));
    Serial.print(DailyCounter);
    Serial.print(F(", EC: "));
    Serial.print(EC);
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
    if(DepthDiff>=1.0){
      Writer = 1;
      Serial.println(F("We will write because DepthDIFF was greater than 1 mbar"));
    }
    if(DailyCounter>=MaxLogInterval){
      Writer = 1;
      Serial.println(F("We will write because the time elapsed was greater than the max logging interval"));
    }
    if(FasterA>=900.0){
      Writer = 1;
      Serial.println(F("We will log because there is a jumper between A5 and 5v"));
    }  
    
    if(Writer==1){
      //ok we should send to the web because we have some changing datasets.
      //turn on the TX pin so the RX of the SIM goes high
      digitalWrite(3,HIGH);
      lastECwritten = EC;
      lastTempwritten = temp;
      lastDepthwritten = Depth;
      SendToWeb222(0);
      Serial.print(F("Sent to web. "));
      //turn off the TX and RX pins so the RX and TX pins of the SIM go low - saves power!
      digitalWrite(3,LOW);
      DailyCounter = 0;
    } else{
      Serial.print(F("Skipped. "));
      //DailyCounter = 0;
    }
    //resent counters
    counter = 0;
    EC = 0;
    temp = 0;
    Depth = 0;
  } 
  Serial.println(", delaying.");
  Sleepy::loseSomeTime(1000 * ScanInterval);
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
  AAA += "EoDC/databases/WriteMe.php?SiteName=ECTEST.csv&T=";
  AAA += temp;
  AAA += "&EC=";
  AAA += EC;
  AAA += "&D=";
  AAA += Depth;
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

void SendToWeb(int abc) {
  a = 0;
  if(abc==1){
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
     
      //OK, the system should now be on. 
      a=0;
      do{a++;}while (sendATcommand(F("AT+IPR=19200"),"OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("ATE0"),"OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("AT&W0"),"OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("AT+CGDCONT=1,\"IP\",\"mdata.net.au\",\"0.0.0.0\""), "OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("AT+CGSOCKCONT=1,\"IP\",\"mdata.net.au\""), "OK",10000) == 0 && a<5);
      a=0;
      do{a++;}while (sendATcommand(F("AT+CSOCKSETPN=1"), "OK",10000) == 0 && a<10);
      a=0;
      do{a++;}while (sendATcommand(F("AT+CHTTPSSTART"), "OK",10000) == 0 && a<2);
      a=0;
  }
  
  do{a++;}while (sendATcommand(F("AT+CHTTPSOPSE=\"www.cartridgerefills.com.au\",80,1"), "OK",10000) == 0 && a<5);
//you may need to move the /EoDC/ to the below line bewfore the "WriteMe.php... so it looks like "EoDC/WriteMe.php.... but try the way i have it first
  AAA = "GET /";
  AAA += "EoDC/databases/WriteMe.php?SiteName=ECTEST.csv&T=";
  AAA += temp;
  AAA += "&EC=";
  AAA += EC;
  AAA += "&D=";
  AAA += Depth;
  AAA += " HTTP/1.1\r\nHost: www.cartridgerefills.com.au:80\r\n\r\n";
  BBB = "AT+CHTTPSSEND=";
  BBB +=AAA.length();

  a=0;
  do{a++;}while (sendATcommand(BBB, ">",10000) == 0 && a<5);

  a=0;
  do{a++;}while (sendATcommand(AAA, "OK",10000) == 0 && a<5);

  delay(1000);

  a=0;
  do{a++;}while (sendATcommand(F("AT+CHTTPSCLSE"), "OK",5000) == 0 && a<3);

  //a=0;
  //do{a++;}while (sendATcommand(F("AT+CHTTPSSTOP"), "OK",10000) == 0 && a<5);

  //a=0;
  //do{a++;}while (sendATcommand(F("AT+CPOF"), "OK",10000) == 0 && a<5);
   
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

void GetTempANDEC(){
  
  //Temperature and Depth
  TempVar = depth_sensor.getTemperature(CELSIUS, ADC_512);
  DepthVar = depth_sensor.getPressure(ADC_4096)/10;
  Serial.print(F("Recorded Temperature = "));
  Serial.print(TempVar);
  Serial.print(F("Recorded Depth = "));
  Serial.print(DepthVar);
  
  //EC
  ECVar = 0.0;
  if (polarityEC == true){ //need to do computation with ec var cause its no longer symetric 
  digitalWrite(A1,LOW);
  digitalWrite(A3,HIGH);
  delay(100);
  ECVar = analogRead(A0);
  digitalWrite(A1,LOW);
  digitalWrite(A3,LOW);
  
  polarityEC = false;
  }else{
  digitalWrite(A1,HIGH);
  digitalWrite(A3,LOW);
  delay(100);
  ECVar = analogRead(A0);
  digitalWrite(A1,LOW);
  digitalWrite(A3,LOW);
  
  polarityEC = true; 
  }
  Serial.print(F(", Recorded EC = "));
  Serial.print(ECVar);
  Serial.print(F(", A5 reading = "));
  Serial.print(FasterA);
  
}
