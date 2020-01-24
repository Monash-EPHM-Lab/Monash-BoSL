//todo add check for if i2c device is present to change error code

#include <avr/power.h>
#include <SoftwareSerial.h>
#include <LowPower.h>
#include "src/MS5803.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

#define CHARBUFF 196 //SIM7000 serial response buffer //longer than 255 will cause issues
#define MAXTRASMITINTERVAL 18000//milli seconds

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 3 // Microcontroller RX
#define BOSL_TX 2 // Microcontroller TX

//Site specific config
#define SITEID "BoSL_19817"
#define APN "telstra.internet"

//default variable array (complilation purposes only)
bool defaultVars[6] = {1,1,1,1,1,1};

//millis timer variable 
extern volatile unsigned long timer0_millis;

//gobal variables 
char response[CHARBUFF]; //sim7000 serial response buffer
uint32_t lstTransmit = 0; //timestamp of last transmit (milli seconds)
String dataStr; //Transmit URL

//GPS reponse stings
char temp[19];
char press[11];
char EC[12];
char air[11];
char Nview[3];
char CBC[5];
//previous GPS reponse strings
char Lsttemp[19];
char Lstpress[11];
char LstEC[12];
char Lstair[11];
char LstNview[3];

//SIM7000 serial object
SoftwareSerial simCom = SoftwareSerial(BOSL_RX, BOSL_TX);

//DEFINE MS5803//
MS5803 MyMS5803(ADDRESS_HIGH);
MS5803 MyAirMS5803(ADDRESS_LOW);

//DEFINE DS18B20//
OneWire oneWire(A2);
DallasTemperature MyDS18B20(&oneWire);
 
////clears char arrays////
void charBuffclr(bool clrVars[6] = defaultVars){
    if(clrVars[0]){
    memset(temp, '\0', 19);
    }
    if(clrVars[1]){
    memset(press, '\0', 11);
    }
    if(clrVars[2]){
    memset(EC, '\0', 12);
    }
    if(clrVars[3]){
    memset(air, '\0', 11);
    }
    if(clrVars[4]){
    memset(Nview, '\0', 3); 
    }
    if(clrVars[5]){
    memset(CBC, '\0', 5); 
    }
}

////clears char arrays////
void LstcharBuffclr(bool clrVars[6] = defaultVars){
    if(clrVars[0]){
    memset(Lsttemp, '\0', 19);
    }
    if(clrVars[1]){
    memset(Lstpress, '\0', 11);
    }
    if(clrVars[2]){
    memset(LstEC, '\0', 12);
    }
    if(clrVars[3]){
    memset(Lstair, '\0', 11);
    }
    if(clrVars[4]){
    memset(LstNview, '\0', 3);
    }
}

////stores coordinate data as previous and clears current arrays////
void charBuffAdvance(bool advVars[6] = defaultVars){
    uint8_t i;
    
    if(advVars[0]){
        for(i = 0; i < 19; i++){
            Lsttemp[i] = temp[i];
        }
    }
    if(advVars[1]){
        for(i = 0; i < 11; i++){
            Lstpress[i] = press[i];
        }
    }
    if(advVars[2]){
        for(i = 0; i < 12; i++){
            LstEC[i] = EC[i];
        }
    }
    if(advVars[3]){
        for(i = 0; i < 11; i++){
            Lstair[i] = air[i];
        }
    }
    if(advVars[4]){
        for(i = 0; i < 3; i++){
            LstNview[i] = Nview[i];
        }
    }
   
    bool clrVars[6] = {1,1,1,1,1,0};
    
    charBuffclr(clrVars);
}

////reads battery voltage from responce char array////
void storeCBCresponse(){
    
    bool end = 0;
    uint8_t x = 0;
    uint8_t j = 0;
    
    bool clrVars[6] = {0,0,0,0,0,1};
    
    charBuffclr(clrVars);
    
    //loop through reponce to extract data
    for (uint8_t i=0; i < CHARBUFF; i++){

        //string splitting cases
        switch(response[i]){
        case ':':
            x = 9;
            j=0;
            i += 2;
            break;

        case ',':
            x++;
            j=0;
            break;

        case '\0':
            end = 1;
            break;
        case '\r':
            x++;
            j=0;
            break;
        }
        //write to char arrays
        if (response[i] != ','){
            switch(x){
                case 11:
                    CBC[j] = response[i];
                break;             
            }
            //increment char array counter
            j++;
        }
        //break loop when end flag is high
        if (end){
            i = CHARBUFF; 
        }
    }

}

void setup() {
    
  //EC DEFINE PINS//
    pinMode(A0,INPUT); //EC PIN is A0
    pinMode(A3,OUTPUT);//EC Power is A3
    pinMode(A1,OUTPUT);//EC Ground is A1
    digitalWrite(A1,LOW);//EC Ground is set to low 
        
  //clear buffers
  charBuffclr();
  LstcharBuffclr();
  
  //ensure sim is in the off state
  simOff();
  
  //begin serial
  Serial.begin(BAUDRATE);
  simCom.begin(BAUDRATE);

  Serial.println("Initialising SIM 7000");
  //initialise sim (on arduino startup only)
	 simOn();
     simInit();
        
     netReg();
     netUnreg();

	 simOff();
}
    
void loop() {
      
      charBuffclr();
      
      Serial.println("EC");
      ECread();
      Serial.println("PRESS");
      pressread();
      Serial.println("TEMP");
      tempread();
      Serial.println("AIR");
      airread();
      
      if(shouldTrasmit()){
            simOn();
            
            netUnreg();
            CBCread();
			
            Transmit(); 

            simOff();
      }
    
  
    Serial.println("Sleep");
	Serial.println();
	
	Sleepy(60);
}

void ECread(){
  int ECVar;
  //EC
  ECVar = 0;
  digitalWrite(A3,HIGH);
  ECVar = analogRead(A0);
  ECVar = analogRead(A0);// This is not a mistake, First reading will be low beause if charged a capacitor
  digitalWrite(A3,LOW);
  
  itoa(ECVar, EC, 10);
}

void pressread(){
  float pressvar;
  uint32_t pressint;
  
  MyMS5803.reset();
  MyMS5803.begin();
  
  pressvar = MyMS5803.getPressure(ADC_4096);

  pressint = (int)10*pressvar;

  ltoa(pressint, press, 10);
  
  //error code value B1004
  if (pressvar == 0){
	  press[0] = 'B';
	  press[1] = '1';
	  press[2] = '0';
	  press[3] = '0';
	  press[4] = '4';
  }
  
}

void airread(){
  float airvar;
  uint32_t airint;
  
  MyAirMS5803.reset();
  MyAirMS5803.begin();
  
  airvar = MyAirMS5803.getPressure(ADC_4096);

  airint = (int)10*airvar;
  
  ltoa(airint, air, 10);
  
  //error code value B1710 
  if (airvar == 0){
	  air[0] = 'B';
	  air[1] = '1';
	  air[2] = '7';
	  air[3] = '1';
	  air[4] = '0';
  }
}


void tempread(){
  float tempvar;
  
  MyDS18B20.begin();
  MyDS18B20.requestTemperatures();
  
  tempvar = MyDS18B20.getTempCByIndex(0);
  
  itoa(int(100*tempvar), temp, 10);

  if(tempvar == -127){
	temp[0] = 'B';
	temp[1] = '1';
	temp[2] = '5';
	temp[3] = '1';
	temp[4] = '2';
	temp[5] = '\0';
  }
}



////SLEEPS FOR SET TIME////
void Sleepy(uint16_t tsleep){ //Sleep Time in seconds
    
    simCom.flush(); // must run before going to sleep
 	
    Serial.flush(); // ensures that all messages have sent through serial before arduino sleeps

    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); //8 seconds dosen't work on the 8mhz
    //advance millis timer as it is paused in sleep
    noInterrupts();
    timer0_millis += 4000;
    interrupts(); 
    
    
    while(tsleep >= 8){
        LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); //8 seconds dosen't work on the 8mhz
        //advance millis timer as it is paused in sleep
        noInterrupts();
        timer0_millis += 4000;
        interrupts();
        
        tsleep -= 4;
    }
}

////TRANSMITS LAST GPS CORDINATES TO WEB////
void Transmit(){
    
    dataStr = "AT+HTTPPARA=\"URL\",\"www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=";
    
    dataStr += SITEID;
    dataStr += ".csv&T=";
    dataStr += temp;
    dataStr += "&D=";
    dataStr += press;
    dataStr += "&V=";
    dataStr += air;
    dataStr += "&EC=";
    dataStr += EC;
    dataStr += "&B=";
    dataStr += CBC;
    dataStr += "\"";
    
    netReg();
    
    
    ///***check logic
   //set CSTT - if it is already set, then no need to do again...
        sendATcmd(F("AT+CSTT?"), "OK",1000);   
        if (strstr(response, "telstra.internet") != NULL){
            //this means the cstt has been set, so no need to set again!
            Serial.println("CSTT already set to APN ...no need to set again");
       } else {
            sendATcmd(F("AT+CSTT=\"telstra.internet\""), "OK",1000);
        }
    
    
    //close open bearer
    sendATcmd(F("AT+SAPBR=2,1"), "OK",1000);
    if (strstr(response, "1,1") == NULL){
        if (strstr(response, "1,3") == NULL){
        sendATcmd(F("AT+SAPBR=0,1"), "OK",1000);
        }
        sendATcmd(F("AT+SAPBR=1,1"), "OK",1000);
    }
    
    sendATcmd(F("AT+HTTPINIT"), "OK",1000);
    sendATcmd(F("AT+HTTPPARA=\"CID\",1"), "OK",1000);
   
    sendATcmd(dataStr, "OK",1000);
   
   sendATcmd(F("AT+HTTPACTION=0"), "200",2000);
    sendATcmd(F("AT+HTTPTERM"), "OK",1000);
  //close the bearer connection
    sendATcmd(F("AT+SAPBR=0,1"), "OK",1000);
    
    netUnreg();
    
    lstTransmit = millis();
    charBuffAdvance();
}

////RETURNS TRUE IF SIM7000 SHOULD TRANSMIT////
bool shouldTrasmit(){   
    //checks to see if it has been longer than max transmit interval
    if (MAXTRASMITINTERVAL < millis() - lstTransmit){
        return 1;
    }
    
    //if all checks for transmit are not nessesary, return false
    return 0;
}


////power down cellular functionality////
void netUnreg(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
}

////register to network////
void netReg(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "+CPIN: READY", 1000) == 0){
        sendATcmd(F("AT+CFUN=6"), "OK", 10000);
        xDelay(10000);
        
        sendATcmd(F("AT+CFUN=1"), "OK", 1000);
    }
    xDelay(2000);
    sendATcmd(F("AT+CREG?"), "+CREG: 0,1", 2000);
}

////sends at command, checks for reply////
bool sendATcmd(String ATcommand, char* expctAns, unsigned int timeout){
    uint32_t timeStart;
    bool answer;
    uint8_t a=0;
    
    do{a++;
    
    Serial.println(ATcommand);
    
    answer=0;
    
    timeStart = 0;


    delay(100);

    while( simCom.available() > 0) {
        simCom.read();    // Clean the input buffer
    }
    
    simCom.println(ATcommand);    // Send the AT command 


    uint8_t i = 0;
    timeStart = millis();
    memset(response, '\0', CHARBUFF);    // Initialize the string

    // this loop waits for the answer

    do{
        if(simCom.available() != 0){    
            response[i] = simCom.read();
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



////initialises sim on arduino startup////
void simInit(){
   
      sendATcmd(F("AT+IPR=9600"),"OK",1000);
      
      sendATcmd(F("ATE0"),"OK",1000);
      
      sendATcmd(F("AT&W0"),"OK",1000);
  
}


////powers on SIM7000////
void simOn() {
    //do check for if sim is on
	pinMode(PWRKEY, OUTPUT);
	pinMode(BOSL_TX, OUTPUT);
	digitalWrite(BOSL_TX, HIGH);
	pinMode(BOSL_RX, INPUT_PULLUP);


	digitalWrite(PWRKEY, LOW);
	// See spec sheets for your particular module
	xDelay(1000); // For SIM7000

	digitalWrite(PWRKEY, HIGH);
    xDelay(4000);
}

////powers off SIM7000////
void simOff() {
	//  TX / RX pins off to save power

	digitalWrite(BOSL_TX, LOW);
	digitalWrite(BOSL_RX, LOW);

	digitalWrite(PWRKEY, LOW);

	// See spec sheets for your particular module
	xDelay(1200); // For SIM7000

	digitalWrite(PWRKEY, HIGH);
    xDelay(2000);
}

void CBCread(){
    //get GNSS data
    if (sendATcmd(F("AT+CBC"), "OK",1000)){
        
        storeCBCresponse();
        
    }
}

//like delay but lower power and more dodgy//
void xDelay(uint32_t tmz){
	uint32_t tmzslc = tmz/64;
	//64 chosen as ballance between power savings and time spent in full clock mode
	clock_prescale_set(clock_div_64);
		delay(tmzslc);
	clock_prescale_set(clock_div_1);
	
	cli();
	timer0_millis += 63*tmzslc;	
	sei();
	
	delay(tmz-64*tmzslc);
}













































