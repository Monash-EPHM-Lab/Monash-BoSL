

//Notes may need to adjust APN to get Cellular connectivity test to pass.


#include <SoftwareSerial.h>
#include <avr/power.h>
#include <LowPower.h>


#define SITEID F("BoSL_rev_0_4_1_SELF_TEST")

#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

#define CHARBUFF 196 //SIM7000 serial response buffer //longer than 255 will cause issues

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 9 // Microcontroller RX
#define BOSL_TX 8 // Microcontroller TX



extern volatile unsigned long timer0_millis;

char response[CHARBUFF];
char CBC[5];
char ok[3] = {'O', 'K', '\0'};  

uint8_t results[9] = {0,0,0,0,0,0,0,0,0};


String dataStr;

//SIM7000 serial object
SoftwareSerial simCom = SoftwareSerial(BOSL_RX, BOSL_TX);



void setup() {
          
      //begin serial
      Serial.begin(BAUDRATE);
      simCom.begin(BAUDRATE);
	  Serial.println("#############BoSL TEST############");

	  simInit();
	  CBCread();
	  transmit();
}
    
void loop() { 

}





bool cellCheck(){
	bool success = 0;

	netUnreg();

	CBCread();

	success = transmit(); 
	
	return success;
}



bool simCheck(){
    if(sendATcmd(F("AT"), ok, 1000)){
    return 1;
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

bool transmit(){
    bool success = 0;
	
    dataStr = F("AT+HTTPPARA=\"URL\",\"www.bosl.com.au/IoT/testing/scripts/WriteMe.php?SiteName=");
    
    dataStr += SITEID;
    dataStr += F(".csv&T=");
    dataStr += CBC;
    dataStr += F("\"");
    
    netReg();
    
    
    ///***check logic
   //set CSTT - if it is already set, then no need to do again...
        sendATcmd(F("AT+CSTT?"), ok,1000);   
        if (strstr(response, "mdata.net.au") != NULL){
            //this means the cstt has been set, so no need to set again!
            Serial.println(F("CSTT already set to APN ...no need to set again"));
       } else {
            sendATcmd(F("AT+CSTT=\"mdata.net.au\""), ok,1000);
        }
    
    
    //close open bearer
    sendATcmd(F("AT+SAPBR=2,1"), ok,1000);
    if (strstr(response, "1,1") == NULL){
        if (strstr(response, "1,3") == NULL){
        sendATcmd(F("AT+SAPBR=0,1"), ok,1000);
        }
        sendATcmd(F("AT+SAPBR=1,1"), ok,1000);
    }
    
    sendATcmd(F("AT+HTTPINIT"), ok,1000);
    sendATcmd(F("AT+HTTPPARA=\"CID\",1"), ok,1000);
   
    sendATcmd(dataStr, ok,1000);
   
   sendATcmd(F("AT+HTTPACTION=0"), "200",2000);
    sendATcmd(F("AT+HTTPTERM"), ok,1000);
  //close the bearer connection
    success = sendATcmd(F("AT+SAPBR=0,1"), ok,1000);
    
    netUnreg();
	
	return success;
}

////initialises sim on arduino startup////
void simInit(){
    
  simOn();
  
      sendATcmd(F("AT+IPR=9600"),ok,1000);
      
      sendATcmd(F("ATE0"),ok,1000);
      
      sendATcmd(F("AT&W0"),ok,1000);

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


void CBCread(){
    //get GNSS data
    if (sendATcmd(F("AT+CBC"), ok,1000)){
        
        storeCBCresponse();
        
    }
}

void storeCBCresponse(){
    
    bool end = 0;
    uint8_t x = 0;
    uint8_t j = 0;
    
    memset(CBC, '\0', 5);
    
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

void netUnreg(){
    sendATcmd(F("AT+CFUN=0"), ok, 1000);
}

////register to network////
void netReg(){
    sendATcmd(F("AT+CFUN=0"), ok, 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "+CPIN: READY", 1000) == 0){
        sendATcmd(F("AT+CFUN=6"), ok, 10000);
        xDelay(10000);
        
        sendATcmd(F("AT+CFUN=1"), ok, 1000);
    }
    xDelay(2000);
    sendATcmd(F("AT+CREG?"), "+CREG: 0,1", 2000);
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

bool gnssCheck(){
    uint32_t maxTFF = 900000;
    bool answer;

    uint32_t timeStart;
    
    const bool updateVars[6] = {0,0,0,0,1,0};
    
    //turn on GNSS functionality//
    sendATcmd(F("AT+CGNSPWR=1"), ok, 1000);
    
    //timeout counter start
    timeStart = millis();
    answer = 0;


    
    timeStart = millis();
    answer = 0;
    
   
        //wait for fix
        while(maxTFF > millis() - timeStart){
            //check for fix
            answer = sendATcmd(F("AT+CGNSINF"), "F: 1,1",1000);

            if(answer){
                return 1;
            }  
        }
 
    return 0;

}