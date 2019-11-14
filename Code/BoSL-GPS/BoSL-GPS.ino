#include <Wire.h>
#include <SoftwareSerial.h>

#include <LowPower.h> 

#include <SparkFun_MS5803_I2C.h>

#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 3 // Microcontroller RX
#define BOSL_TX 2 // Microcontroller TX

String SiteID = "developement";

SoftwareSerial SIM7000 = SoftwareSerial(BOSL_RX, BOSL_TX);

char response[100];

//Declaring variables to store results
float EC, Pressure, Pressure_sum, Temp, Temp_sum, FasterA, batLev;
short Counter;
int a, ScanInterval, LogInterval, Writer;
String AAA, BBB;

unsigned long previous;
uint8_t x, answer;

void setup() {
 
  Serial.begin(BAUDRATE);
  SIM7000.begin(BAUDRATE);
  

  SendToWeb222(1);

}

void loop() {
  


      //turn on SIM7000
      digitalWrite(PWRKEY,HIGH);
      SendToWeb222(0);
      //turn off SIM7000


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
