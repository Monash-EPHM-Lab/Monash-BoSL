#include <SoftwareSerial.h>
#include "SensorData.h"
#include "SensorEC.h"
#include "SIM7000.h"



#define SIMCOM_7000 // SIM7000A/C/E/G

// For SIM7000 BoSL board
#define PWRKEY 6
#define DTR 8 // Connect with solder jumper
#define BOSL_TX 3 // Microcontroller RX
#define BOSL_RX 2 // Microcontroller TX


#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

SoftwareSerial boslSS = SoftwareSerial(BOSL_TX, BOSL_RX);

SIM7000 bosl = SIM7000();


uint8_t type;

const int ECPinR = A0;
const int ECPinA = A1;
const int ECPinB = A3;

const int diffThreshold = 0.1;  //10% change in consecutive readings before transmitting

uint16_t counter = 0;


SensorEC EC = SensorEC(ECPinR, ECPinA, ECPinB, true);  //three pins and switching is true.

//SensorData data = SensorData("startfile.txt"); //initialise data array on SD card


///////////////////////////////////////////////////////////////
void setup() {
	pinMode(PWRKEY, OUTPUT);
  	pinMode(DTR, OUTPUT);

  	// Turn on the module by pulsing PWRKEY low for a little bit
  	// This amount of time depends on the specific module that's used
  	powerOn(); // See function definition at the very end of the sketch

  	Serial.begin(BAUDRATE);
  	Serial.println(F("BoSL Initialising"));

  	// ensure that board is set to 9600 baudrate from 115200 default. 
  	boslSS.begin(115200); // Default SIM7000 shield baud rate
 	Serial.println(F("Configuring to 9600 baud"));
  	boslSS.println("AT+IPR=9600"); // Set baud rate. Manually change to BAUDRATE if modifying
  	delay(100); // Short pause to let the command run

  	boslSS.begin(BAUDRATE);
  	if (! bosl.begin(boslSS)) {
    	Serial.println(F("Couldn't find BOSL"));
    	while (1); // Don't proceed if it couldn't find the device
  	}


  	type = bosl.type();
  	Serial.println(F("BoSL is OK"));
  	Serial.print(F("Found "));
  	switch (type) {
    	case SIM7000G:
      		Serial.println(F("SIM7000G (Default BoSL)")); break;
    	default:
      		Serial.println(F("???")); break;
    }

  	// Set modem to full functionality
  	bosl.setFunctionality(1); // AT+CFUN=1

  	// Configure a GPRS APN, username, and password.
  	// You might need to do this to access your network's GPRS/data
  	// network.  Contact your provider for the exact APN, username,
  	// and password values.  Username and password are optional and
  	// can be removed, but APN is required.
  	//bosl.setNetworkSettings(F("your APN"), F("your username"), F("your password"));
  	bosl.setNetworkSettings(F("telstra.internet")); // For Telstra (Australia) SIM card - CAT-M1 (Band 28)
  	//bosl.setNetworkSettings(F("yesinternet")); // For optus card
  	//bosl.setNetworkSettings(F("mdata.net.au")); // For aldi card

  	bosl.setPreferredLTEMode(1); // Use LTE CAT-M only, not NB-IoT
  	bosl.setOperatingBand("CAT-M", 28); // Telstra uses band 28



  //	data.dump(); // dump SD card file
}


///////////////////////////////////////////////////////////////
void loop() {
	delay(1000);
	Serial.print("count: ");
	Serial.println(counter);
	//delay(10000);
	counter++;

	EC.measure(); //take measurement every 10sec
/*	temp.measure();
	depth.measure();	*/

	if (counter % 6 == 5) { //one minute has passed
		logData();
		//transmitCheck();
	}
}


///////////////////////////////////////////////////////////////
void logData() { // save all data to last variable and prepare for new data

	float ECVal = EC.getAverage(); 
	//data.composeLine(ECVal, EC_TAG); //for SD card
	EC.clearSum();   //clear the EC sensor value to start a new average.

/*	float tempVal = temp.getAverage(); 
	data.composeLine(tempVal, TEMP_TAG);
	temp.clearSum();   

	float depthVal = depth.getAverage(); 
	data.composeLine(depthVal, DEPTH_TAG);
	depth.clearSum();    */

	//data.writeLine(); //write the composed line to SD card
}


///////////////////////////////////////////////////////////////
void transmitCheck() {

	float ECdiff = (EC.getLast() - EC.getAverage()) / EC.getLast(); 
	// EC percentage change: (old - new) / old


/*	float tempDiff = (temp.getLast() - temp.getAverage()) / temp.getLast(); 

	float depthDiff = (depth.getLast() - depth.getAverage()) / depth.getLast(); */

//	if (ECdiff >= diffThreshold || tempDiff >= diffThreshold || depthDiff >= diffThreshold) {
	if (ECdiff >= diffThreshold) {
//		transmit(); //make this do something....
	}
} 


void transmit() {
	char URL[150];

	sprintf(URL, "www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=development.csv&T=SIM7000&EC=TEST"); // No need to specify http:// or https://

	if (!bosl.postData("GET", URL)) {
    	Serial.println(F("Failed to complete HTTP GET to EoDC database..."));
	}
        
}


void openNetwork() {
	// test all network functions on SIM
	const char CFUN[] = "AT+CFUN?";
	const char OK[] = "";
	bosl.sendCheckReply(CFUN, OK, 30000);
	delay(100);

	const char CREG[] = "AT+CREG?";
	bosl.sendCheckReply(CREG, OK, 30000);
	delay(100);

	const char CGREG[] = "AT+CGREG?";
	bosl.sendCheckReply(CGREG, OK, 30000);
	delay(100);

	const char C4[] = "AT+CPSI?";
	bosl.sendCheckReply(C4, OK, 30000);
	delay(100);

	const char C5[] = "AT+CGDCONT?";
	bosl.sendCheckReply(C5, OK, 30000);
	delay(100);

	const char C6[] = "AT+COPS?";
	bosl.sendCheckReply(C6, OK, 60000);
	delay(100);
	// turn GPRS on
	if (!bosl.enableGPRS(true)) {
		Serial.println(F("Failed to turn on"));
	}
} 




void powerOn() {
  digitalWrite(PWRKEY, LOW);

  delay(100); // For SIM7000
  
  digitalWrite(PWRKEY, HIGH);
}

