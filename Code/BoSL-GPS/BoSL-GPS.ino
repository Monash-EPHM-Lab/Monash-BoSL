#include <SoftwareSerial.h>
#include <LowPower.h>


#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

#define CHARBUFF 196 //SIM7000 serial response buffer //longer than 255 will cause issues
#define MAXTRASMITINTERVAL 900000//milli seconds

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 3 // Microcontroller RX
#define BOSL_TX 2 // Microcontroller TX

//Site specific config
#define SITEID "GPS"
#define SITEPINGID "GPSPING"
#define APN "telstra.internet"

//millis timer variable 
extern volatile unsigned long timer0_millis;

//gobal variables 
char response[CHARBUFF]; //sim7000 serial response buffer
uint32_t lstTransmit; //timestamp of last transmit (milli seconds)
uint32_t lstPing; //timestamp of last ping (milli seconds)
String dataStr; //Transmit URL

//GPS reponse stings
char UTC[19];
char lat[11];
char lng[12];
char CN0[3];
char Nview[3];
//previous GPS reponse strings
char LstUTC[19];
char Lstlat[11];
char Lstlng[12];
char LstCN0[3];
char LstNview[3];

//SIM7000 serial object
SoftwareSerial simCom = SoftwareSerial(BOSL_RX, BOSL_TX);
 

void setup() {
        
  //clear buffers
  charBuffclr();
  LstcharBuffclr();
  
  //ensure sim is in the off state
  simOff();
  
  //begin serial
  Serial.begin(BAUDRATE);
  simCom.begin(BAUDRATE);

  Serial.println("initialising sim");
  //initialise sim (on arduino startup only)
     simInit();
        
     netReg();
     netUnreg();

}

    
    
void loop() {
        
     simOn();
     netUnreg();
     
      if(GNSSgetFix(120000)){
      
          
          //read latest GPS coordinates
          GNSSread();
          

          //check if transmit is nesseary
          if(shouldTrasmit()){
            Transmit();  
          }
      
      }else{
          
          if(shouldPing()){
              Ping();
          }
      }
  
    Serial.println("Sleep");

    Sleepy(300);
 
}

////PINGS SERVER TO SHOW IF STILL ALIVE////
void Ping(){
        
    dataStr = "AT+HTTPPARA=\"URL\",\"www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=";
    
   
    dataStr += SITEPINGID;
    dataStr += ".csv&D=";
    dataStr += "PING\"";
    
    simOn();
    
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
    
  
  
   // sendATcmd(F("AT+HTTPTERM"), "OK",10000); *ERROR
    
    sendATcmd(F("AT+HTTPINIT"), "OK",1000);
    sendATcmd(F("AT+HTTPPARA=\"CID\",1"), "OK",1000);
    
    //sendATcmd(F("AT+HTTPPARA=\"URL\",\"www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=" + SITEID + ".csv&Z=" + UTC + "&Y=" + lat + "&X=" + lng + "&W=" + CN0 + "&V=" + Nview + "\""), "OK",1000);
    sendATcmd(dataStr, "OK",1000);
   
   sendATcmd(F("AT+HTTPACTION=0"), "200",2000);
    sendATcmd(F("AT+HTTPTERM"), "OK",1000);
  //close the bearer connection
    sendATcmd(F("AT+SAPBR=0,1"), "OK",1000);
    
    netUnreg();
 
    //record transmision details 
    lstPing = millis();
}

////RETURNS TRUE IF SIM7000 SHOULD PING SERVER////
bool shouldPing(){
    
    
    //checks to see if it has been longer than max transmit interval
    if ((MAXTRASMITINTERVAL < millis() - lstTransmit) && (MAXTRASMITINTERVAL < millis() - lstPing)){
        return 1;
    }
    
    //if all checks for transmit are not nessesary, return false
    return 0;
}


////SLEEPS FOR SET TIME////
void Sleepy(uint16_t tsleep){ //Sleep Time in seconds
    
    simCom.flush(); // must run before going to sleep
 	
    Serial.flush(); // ensures that all messages have sent through serial before arduino sleeps
    simOff();

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
    delay(50);
    //advance millis timer as it is paused in sleep
    noInterrupts();
    timer0_millis += 8000;
    interrupts(); 
    
    
    while(tsleep >= 16){
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
        delay(50);
        //advance millis timer as it is paused in sleep
        noInterrupts();
        timer0_millis += 8000;
        interrupts();
        
        tsleep -= 8;
    }
}

////TRANSMITS LAST GPS CORDINATES TO WEB////
void Transmit(){
    
    
    dataStr = "AT+HTTPPARA=\"URL\",\"www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=";
    
   
    dataStr += SITEID;
    dataStr += ".csv&Z=";
    dataStr += UTC;
    dataStr += "&Y=";
    dataStr += lat;
    dataStr += "&X=";
    dataStr += lng;
    dataStr += "&T=";
    dataStr += CN0;
    dataStr += "&V=";
    dataStr += Nview;
    
    dataStr += "\"";
    
    simOn();
    
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
    
  
  
   // sendATcmd(F("AT+HTTPTERM"), "OK",10000); *ERROR
    
    sendATcmd(F("AT+HTTPINIT"), "OK",1000);
    sendATcmd(F("AT+HTTPPARA=\"CID\",1"), "OK",1000);
    
    //sendATcmd(F("AT+HTTPPARA=\"URL\",\"www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=" + SITEID + ".csv&Z=" + UTC + "&Y=" + lat + "&X=" + lng + "&W=" + CN0 + "&V=" + Nview + "\""), "OK",1000);
    sendATcmd(dataStr, "OK",1000);
   
   sendATcmd(F("AT+HTTPACTION=0"), "200",2000);
    sendATcmd(F("AT+HTTPTERM"), "OK",1000);
  //close the bearer connection
    sendATcmd(F("AT+SAPBR=0,1"), "OK",1000);
    
    netUnreg();
    
    
    
    
    //record transmision details 
    lstTransmit = millis();
    charBuffAdvance();
}

////RETURNS TRUE IF SIM7000 SHOULD TRANSMIT////
bool shouldTrasmit(){
    uint8_t i;
    
    //if buffer is empty don't transmit
    if(UTC[0] == '\0') {
        return 0;
    }
    
    //latitude is char array so this checks to see if most significant digits are the same 
    for(i=0; i<7; i++){
        if (lat[i] != Lstlat[i]){
            return 1;
        }
    }
    //longitude is char array so this checks to see if most significant digits are the same 
    for(i=0; i<7; i++){
        if (lng[i] != Lstlng[i]){
            return 1;
        }
    }
    
    //checks to see if it has been longer than max transmit interval
    if (MAXTRASMITINTERVAL < millis() - lstTransmit){
        return 1;
    }
    
    //if all checks for transmit are not nessesary, return false
    return 0;
}

////attempts to read GNSS data and stores it the char arrays if new data is available////
void GNSSread(){
    //get GNSS data
    if (sendATcmd(F("AT+CGNSINF"), "OK",1000)){
       
        
        bool end = 0;
        uint8_t x = 0;
        uint8_t j = 0;
        
        //loop through reponce to extract data
        for (uint8_t i=0; i < CHARBUFF; i++){

            //string splitting cases
            switch(response[i]){
            case ':':
                x++;
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
            }
            
           //do nothing if response does not have fix
           if ((x == 2) && ((response[i+1] != '1')&&((response[i] != '1')))){
               end = 1;
           }
           //if fix then clear the char arrays to write new data
           if ((x == 2) && (response[i] == '1')){
               charBuffclr();
           }

            //write to char arrays
            if (response[i] != ','){
                switch(x){
                    case 3:
                    UTC[j] = response[i];
                    break;
                    case 4:
                    lat[j] = response[i];
                    break;
                    case 5:
                    lng[j] = response[i];
                    break;
                    case 15:
                    Nview[j] = response[i];
                    break;
                    case 19:
                    CN0[j] = response[i];
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
}


////Activates GNSS and waits time in argument for coordiante fix////
bool GNSSgetFix(uint32_t maxTFF){
    
    bool answer;
    uint32_t timeStart;
    
    //turn on GNSS functionality//
    sendATcmd(F("AT+CGNSPWR=1"), "OK", 1000);
    
    //timeout counter start
    timeStart = millis();
    answer = 0;
    
    
    while(maxTFF > millis() - timeStart){
    //check for fix
    answer = sendATcmd(F("AT+CGNSINF"), "+CGNSINF: 1,1",1000);
    
    if(answer){
        return 1;
    }
    
    }
    return 0;
}

////powers down GNSS functionality////
void GNSSoff(){
    sendATcmd(F("AT+CGNSPWR=0"), "OK", 1000);
}

////power down cellular functionality////
void netUnreg(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
}

////register to network////
void netReg(){
    sendATcmd(F("AT+CFUN=0"), "OK", 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "OK", 1000) == 0){
        sendATcmd(F("AT+CFUN=6"), "OK", 10000);
        
        sendATcmd(F("AT+CFUN=1"), "OK", 1000);
    }
    delay(2000);
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

////clears char arrays////
void charBuffclr(){
    memset(UTC, '\0', 19);
    memset(lat, '\0', 11);
    memset(lng, '\0', 12);
    memset(Nview, '\0', 3);
    memset(CN0, '\0', 3);   
}

////clears char arrays////
void LstcharBuffclr(){
    memset(LstUTC, '\0', 19);
    memset(Lstlat, '\0', 11);
    memset(Lstlng, '\0', 12);
    memset(LstNview, '\0', 3);
    memset(LstCN0, '\0', 3);
}

////stores coordinate data as previous and clears current arrays////
void charBuffAdvance(){
    uint8_t i;
    
    for(i = 0; i < 19; i++){
        LstUTC[i] = UTC[i];
    }
    for(i = 0; i < 11; i++){
        Lstlat[i] = lat[i];
    }
    for(i = 0; i < 12; i++){
        Lstlng[i] = lng[i];
    }
    for(i = 0; i < 3; i++){
        LstNview[i] = Nview[i];
    }
    for(i = 0; i < 3; i++){
        LstCN0[i] = CN0[i];
    }
   
    charBuffclr();
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



