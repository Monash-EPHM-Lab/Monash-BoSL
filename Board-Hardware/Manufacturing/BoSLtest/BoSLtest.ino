#include <SoftwareSerial.h>
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

#define CHARBUFF 196 //SIM7000 serial response buffer //longer than 255 will cause issues

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 3 // Microcontroller RX
#define BOSL_TX 2 // Microcontroller TX

char response[CHARBUFF];

//SIM7000 serial object
SoftwareSerial simCom = SoftwareSerial(BOSL_RX, BOSL_TX);

MS5803 presSense(ADDRESS_LOW);

void setup() {
          
      //begin serial
      Serial.begin(BAUDRATE);
      simCom.begin(BAUDRATE);

    Serial.println("#######BoSL SELF TEST#######");
    Serial.println("");
  
    //check pressure
    if (presCheck()){
         Serial.println("###/// PRESSURE OK ///###");
         Serial.println("");
     }else{
         Serial.println("###-----> PRESSURE FAIL <-----###");
         Serial.println("");
     }
     
     simInit();
     
     //check SIM 7000
     if (simCheck()){
         Serial.println("###/// SIM 7000 OK ///###");
         Serial.println("");
     }else{
         Serial.println("###-----> SIM 7000 FAIL <-----###");
         Serial.println("");
     }
     
     //check SIM 7000 communication with SIM card
     if (simCDCheck()){
         Serial.println("###/// SIM CARD OK ///###");
         Serial.println("");
     }else{
         Serial.println("###-----> SIM CARD FAIL <-----###");
         Serial.println("");
     }
}
    
void loop() { 
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

bool simCheck(){
    if(sendATcmd(F("AT"), "OK", 1000)){
    return 1;
    }
    
   return 0;
}

bool simCDCheck(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "+CPIN: READY", 1000)){
    return 1;
    }
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "+CPIN: NOT INSERTED", 1000)){
    return 0;
    }
    
    return 0;
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
    
  simOn();
  
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
	delay(100); // For SIM7000

	digitalWrite(PWRKEY, HIGH);
    delay(5000);
}

////powers off SIM7000////
void simOff() {
	//  TX / RX pins off to save power

	digitalWrite(BOSL_TX, LOW);
	digitalWrite(BOSL_RX, LOW);

	digitalWrite(PWRKEY, LOW);

	// See spec sheets for your particular module
	delay(3000); // For SIM7000

	digitalWrite(PWRKEY, HIGH);
    delay(10);
}



