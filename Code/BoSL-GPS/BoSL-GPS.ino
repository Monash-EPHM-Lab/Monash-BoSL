#include <SoftwareSerial.h>
#include <LowPower.h>


#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 3 // Microcontroller RX
#define BOSL_TX 2 // Microcontroller TX


SoftwareSerial simCom = SoftwareSerial(BOSL_RX, BOSL_TX);
 
//Declaring variables to store results
String SiteID = "developement";


uint8_t i;
bool answer;
char response[100];
uint8_t a;

char lat[10];
char lng[11];
char UTC[18];
char CN0[2];
char Nview[2];

void setup() {
  
  simOff();
  
  Serial.begin(BAUDRATE);
  simCom.begin(BAUDRATE);

  Serial.println("initialising sim");
  simInit();

 // netReg();
 // netUnreg();

  GNSSgetFix(100);
  
  GNSSread();
}

    
    
void loop() {
  
 
  
}

void GNSSread(){
    if (sendATcmd(F("AT+CGNSINF"), "OK",1000)){
       //remember to keep old values if new ones are bad
        memset(response, '\0', 100);
    
    x=0;
    for (uint8_t i= 0, i < 100, i++){
        if (response[i] == ","){
            x++;
    }
    
    switch(x){
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        
    }
    }
}

void GNSSgetFix(uint32_t maxTFF){
    uint32_t timeStart;
    
    sendATcmd(F("AT+CGNSPWR=1"), "OK", 1000);
    

    timeStart = millis();
    answer = 0;
    
    while(maxTFF > millis() - timeStart){
    //answer = sendATcmd(F("AT+CGNSINF"), "+CGNSINF: 1,1",1000);
    
    if(answer){
        break;
    }
    
    }
    
}

void GNSSoff(){
    sendATcmd(F("AT+CGNSPWR=0"), "OK", 1000);
}

void netUnreg(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
}

void netReg(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "OK", 1000) == 0){
        sendATcmd(F("AT+CFUN=6"), "OK", 10000);
        
        sendATcmd(F("AT+CFUN=1"), "OK", 1000);
    }
    delay(2000);
    sendATcmd(F("AT+CREG?"), "+CREG: 0,1", 2000);
}

bool sendATcmd(String ATcommand, char* expctAns, unsigned int timeout){
    uint32_t timeStart;
    
    a=0;
    do{a++;
    
    Serial.println(ATcommand);
    answer=0;
    
    timeStart = 0;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( simCom.available() > 0) {
        simCom.read();    // Clean the input buffer
    }
    
    simCom.println(ATcommand);    // Send the AT command 


    i = 0;
    timeStart = millis();

    // this loop waits for the answer
    do{
        if(simCom.available() != 0){    
            response[i] = simCom.read();
            delay(3);
            i++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expctAns) != NULL)    
            {
                answer = 1;
            }
        }    
            
            
        
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - timeStart) < timeout)); 

    if (expctAns == "0"){
                answer = 1;
            }
            
    Serial.println(response);
    
    
    }while(answer == 0 && a < 5);
    
     a = 0;
     return answer;
}



void simInit(){
    
  simOn();
  
      sendATcmd(F("AT+IPR=9600"),"OK",1000);
      
      sendATcmd(F("ATE0"),"OK",1000);
      
      sendATcmd(F("AT&W0"),"OK",1000);
  
}

void simOn() {
	pinMode(PWRKEY, OUTPUT);
	pinMode(BOSL_TX, OUTPUT);
	digitalWrite(BOSL_TX, HIGH);
	pinMode(BOSL_RX, INPUT_PULLUP);


	digitalWrite(PWRKEY, LOW);
	// See spec sheets for your particular module
	delay(100); // For SIM7000

	digitalWrite(PWRKEY, HIGH);
}


///////////////////////////////////////////////////////////////
void simOff() {
	//  TX / RX pins off to save power

	digitalWrite(BOSL_TX, LOW);
	digitalWrite(BOSL_RX, LOW);

	digitalWrite(PWRKEY, LOW);
	// See spec sheets for your particular module
	delay(3000); // For SIM7000
	digitalWrite(PWRKEY, HIGH);
}



