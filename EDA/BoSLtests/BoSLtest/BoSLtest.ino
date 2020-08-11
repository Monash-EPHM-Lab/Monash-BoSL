//This is a script to test the BoSL rev 0.4.1
//Tests inlude:
/*
U2 Integrated MS5803
U7 RTC
BT1 Battery Backup
IC2 SIM7000
I1 SIM Card
AT3 Cellular Connectivity
AT2 GNSS Connectivity
J6 Eithernet connected for BoSL probe
CON1 SD Card
*/

//Notes may need to adjust APN to get Cellular connectivity test to pass.
//test needs to be run twice to check if RTC battery has worked.

#include <SoftwareSerial.h>
#include "src/MS5803.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/power.h>
#include <LowPower.h>
#include <MCP7940.h>
#include <SPI.h>
#include <SD.h>

#define SITEID F("BoSL_rev_0_4_1_SELF_TEST")

#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

#define CHARBUFF 100 //SIM7000 serial response buffer //longer than 255 will cause issues

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 9 // Microcontroller RX
#define BOSL_TX 8 // Microcontroller TX
#define SPRINTF_BUFFER_SIZE 32


extern volatile unsigned long timer0_millis;

char response[CHARBUFF];
char CBC[5];
char ok[3] = {'O', 'K', '\0'};
char inputBuffer[SPRINTF_BUFFER_SIZE];  

uint8_t results[9] = {0,0,0,0,0,0,0,0,0};

uint8_t secs; 
uint8_t min;

String dataStr;

//SIM7000 serial object
SoftwareSerial simCom = SoftwareSerial(BOSL_RX, BOSL_TX);

MS5803 presSense(ADDRESS_LOW);
MS5803 presProbe(ADDRESS_HIGH);
OneWire oneWire(A2);
DallasTemperature MyDS18B20(&oneWire);
MCP7940_Class MCP7940; 

Sd2Card card;
SdVolume volume;
SdFile root;


void setup() {
          
      //begin serial
      Serial.begin(BAUDRATE);
      simCom.begin(BAUDRATE);

    Serial.println(F("#######BoSL SELF TEST#######"));
    Serial.println(F(""));
  
    //check pressure
    if (presCheck()){
         Serial.println(F("###/// PRESSURE PASS ///###"));
         Serial.println(F(""));
		 results[0] = 1;
     }else{
         Serial.println(F("###-----> PRESSURE FAIL <-----###"));
         Serial.println(F(""));
     }
     	 
     simInit();
     
     //check SIM 7000
     if (simCheck()){
         Serial.println(F("###/// SIM 7000 PASS ///###"));
         Serial.println(F(""));
		 results[1] = 1;
     }else{
         Serial.println(F("###-----> SIM 7000 FAIL <-----###"));
         Serial.println(F(""));
     }
     
     //check SIM 7000 communication with SIM card
     if (simCDCheck()){
         Serial.println(F("###/// SIM CARD PASS ///###"));
         Serial.println(F(""));
		 results[2] = 1;
     }else{
         Serial.println(F("###-----> SIM CARD FAIL <-----###"));
         Serial.println(F(""));
     }
	 
	//check Cellular Connectivity
	if(cellCheck()){
		 Serial.println(F("###/// CELLULAR CONNECTIVITY PASS ///###"));
         Serial.println(F(""));
		 results[3] = 1;
	}else{
		 Serial.println(F("###-----> CELLULAR CONNECTIVITY FAIL <-----###"));
         Serial.println(F(""));
	}
	
	//check Cellular Connectivity
	if(gnssCheck()){
		 Serial.println(F("###/// GNSS PASS ///###"));
         Serial.println(F(""));
		 results[4] = 1;
	}else{
		 Serial.println(F("###-----> GNSS FAIL <-----###"));
         Serial.println(F(""));
	}
	
	//check Probe Eithernet
	if(probeCheck()){
		 Serial.println(F("###/// RJ45 PROBE PASS ///###"));
         Serial.println(F(""));
		 results[5] = 1;
	}else{
		 Serial.println(F("###-----> RJ45 PROBE FAIL <-----###"));
         Serial.println(F(""));
	}
	
	 //check RTC
	if(rtcCheck()){
		 Serial.println(F("###/// RTC PASS ///###"));
         Serial.println(F(""));
		 results[6] = 1;
	}else{
		 Serial.println(F("###-----> RTC FAIL <-----###"));
         Serial.println(F(""));
	}
	
	 //check SD CARD
	if(sdCheck()){
		 Serial.println("###/// SD CARD PASS ///###");
         Serial.println("");
		 results[7] = 1;
	}else{
		 Serial.println("###-----> SD CARD FAIL <-----###");
         Serial.println("");
	}
	
	//results summary
	Serial.println(F("#######SUMMARY#######"));
	Serial.print(F("PRESSURE:*****************"));
	Serial.println(results[0]);
	Serial.print(F("SIM 7000:................."));
	Serial.println(results[1]);
	Serial.print(F("SIM CARD:*****************"));
	Serial.println(results[2]);
	Serial.print(F("CELLULAR CONNECTIVITY:...."));
	Serial.println(results[3]);
	Serial.print(F("GNSS:*********************"));
	Serial.println(results[4]);
	Serial.print(F("RJ45 PROBE:..............."));
	Serial.println(results[5]);
	Serial.print(F("RTC:**********************"));
	Serial.println(results[6]);
	Serial.print(F("SD CARD:.................."));
	Serial.println(results[7]);
	Serial.print(F("RTC MINUTE:SECOND*********"));
	Serial.print(min);
	Serial.print(":");
	Serial.println(secs);
}
    
void loop() { 
}

bool sdCheck(){
	if (!card.init(SPI_HALF_SPEED, 10)) {
		return 0;
	}

	Serial.println();
  Serial.print(F("Card type:         "));
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println(F("SD1"));
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println(F("SD2"));
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println(F("SDHC"));
      break;
    default:
      Serial.println(F("Unknown"));
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println(F("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card"));
    while (1);
  }

  Serial.print(F("Clusters:          "));
  Serial.println(volume.clusterCount());
  Serial.print(F("Blocks x Cluster:  "));
  Serial.println(volume.blocksPerCluster());

  Serial.print(F("Total Blocks:      "));
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print(F("Volume type is:    FAT"));
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print(F("Volume size (Kb):  "));
  Serial.println(volumesize);
  Serial.print(F("Volume size (Mb):  "));
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print(F("Volume size (Gb):  "));
  Serial.println((float)volumesize / 1024.0);
  return 1;
}

bool rtcCheck(){
   int i = 0;
   bool passed = 0;
   while (!MCP7940.begin() && i < 3) {                                                  
  
    delay(3000); 
	i++;		
  } 
  i = 0;
  Serial.println(F("MCP7940N initialized."));          
  while (!MCP7940.deviceStatus() && i < 3) {                                           
    Serial.println(F("Oscillator is off, turning it on."));                   
    bool deviceStatus = MCP7940.deviceStart();                                
    if (!deviceStatus) {                                                      
      Serial.println(F("Oscillator did not start, trying again."));    
      delay(1000);                                                            
    }// of if-then oscillator didn't start  
		i++;
  } // of while the oscillator is off   
	i = 0;  
                                                                                
  MCP7940.setBattery(true);   

  
  secs = MCP7940.now().second(); 
  min = MCP7940.now().minute(); 
  delay(3000);
  if(MCP7940.now().second() != secs){
	  passed = 1;
  }
  Serial.print(min);
  Serial.print(":");
  Serial.println(secs);

  return passed;
}

bool cellCheck(){
	bool success = 0;

	netUnreg();

	CBCread();

	success = transmit(); 
	
	return success;
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

bool probeCheck(){
	if(presProbeCheck() && tempProbeCheck()){
		return 1;
	}
	return 0;
}

bool presProbeCheck(){
    
    presProbe.reset();
    presProbe.begin();

    int temp = presProbe.getTemperature(CELSIUS, ADC_512);
    int pres = presProbe.getPressure(ADC_4096);
    
    if (temp < 50 && temp > -10 && pres < 12000 && pres > 6000){
        return 1;
    }
    
    return(0);
}

bool tempProbeCheck(){

	MyDS18B20.begin();
	MyDS18B20.requestTemperatures();
  
	int temp = MyDS18B20.getTempCByIndex(0);
    
    if (temp < 50 && temp > -20){
        return 1;
    }
    
    return(0);
}

bool simCheck(){
    if(sendATcmd(F("AT"), ok, 1000)){
    return 1;
    }
    
   return 0;
}

bool simCDCheck(){
    sendATcmd(F("AT+CFUN=0"), ok, 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "N: R", 1000)){
    return 1;
    }
    sendATcmd(F("AT+CFUN=0"), ok, 1000);
    
    if(sendATcmd(F("AT+CFUN=1"), "N: N", 1000)){
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
    
    if(sendATcmd(F("AT+CFUN=1"), "N: R", 1000) == 0){
        sendATcmd(F("AT+CFUN=6"), ok, 10000);
        xDelay(10000);
        
        sendATcmd(F("AT+CFUN=1"), ok, 1000);
    }
    xDelay(2000);
    sendATcmd(F("AT+CREG?"), "G: 0,1", 2000);
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