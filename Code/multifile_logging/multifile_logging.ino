#include <SoftwareSerial.h>
#include <Wire.h>

#include <SparkFun_MS5803_I2C.h> // Click here to get the library
#include <LowPower.h> // Click here to get the library
#include "SensorEC.h"
#include "SIM7000.h"

#define SIMCOM_7000 // SIM7000A/C/E/G
#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable

// For SIM7000 BoSL board
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define BOSL_RX 3 // Microcontroller RX
#define BOSL_TX 2 // Microcontroller TX


SoftwareSerial boslSS = SoftwareSerial(BOSL_RX, BOSL_TX);

SIM7000 bosl = SIM7000(); 

//  ADDRESS_HIGH = 0x76 // ADDRESS_LOW  = 0x77
MS5803 boardSensor(ADDRESS_LOW);
MS5803 cableSensor(ADDRESS_HIGH);

const int ECPinR = A0;
const int ECPinA = A1;
const int ECPinB = A3;

const char sitename[] = "BoSLv02_longTest.csv";

uint16_t counter = 0;

uint8_t reps = 0;
float lastTemp, lastEC;
double lastPressure;  // last true pressure reading. sensor pressure - air pressure
float tempSum = 0, ECSum = 0;
double sensorPressureSum = 0, airPressureSum = 0;
uint16_t batLevel = 4200; //mV


SensorEC EC = SensorEC(ECPinR, ECPinA, ECPinB);  //three pins therefore switching is true.


///////////////////////////////////////////////////////////////
void setup() {
	Wire.begin();

  	Serial.begin(BAUDRATE);
  	Serial.println(F("BoSL Initialising"));

  	// initialise both sensors
	cableSensor.reset();
	boardSensor.reset();
  cableSensor.begin();
  boardSensor.begin();

	pinMode(PWRKEY, OUTPUT);
  	pinMode(DTR, OUTPUT);

  	// Turn on the module by pulsing PWRKEY low for a little bit
  	// This amount of time depends on the specific module that's used
  	simOn(); // See function definition at the very end of the sketch

 	// ensure that board is set to 9600 baudrate from 115200 default. 
  	//boslSS.begin(115200); // Default SIM7000 shield baud rate
 	//Serial.println(F("Configuring to 9600 baud"));
  	//boslSS.println("AT+IPR=9600"); // Set baud rate. Manually change to BAUDRATE if modifying
  	//delay(100); // Short pause to let the command run

  	boslSS.begin(BAUDRATE);
  	if (! bosl.begin(boslSS)) {
    	Serial.println(F("Couldn't find BOSL"));
    	while (1); // Don't proceed if it couldn't find the device
  	}

  	Serial.println(F("BoSL is OK"));
  	Serial.print(F("Found "));

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

  	simOff();


//	if (!bosl.enableGPRS(true)) {
//		Serial.println(F("Failed to turn on"));
//	}


  //	data.dump(); // dump SD card file
}


///////////////////////////////////////////////////////////////
void loop() {
	Serial.print("count: ");
	Serial.println(counter);

	boslSS.flush(); // must run before going to sleep
	Serial.flush(); // ensures that all messages have sent through serial before arduino sleeps

	LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
	LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);  

	counter++;  // increment these in loop only
	reps++; //this must come before logData();

	ECSum += EC.measure(); //take measurement every 10sec
	tempSum += cableSensor.getTemperature(CELSIUS, ADC_512);
	sensorPressureSum += cableSensor.getPressure(ADC_4096);	
	airPressureSum += boardSensor.getPressure(ADC_4096);	
  Serial.println(airPressureSum);
// TODO: bat level

	if (reps == 6) { //one minute has passed
		logAndTransmitCheck();
	}
}


///////////////////////////////////////////////////////////////
void logAndTransmitCheck() {  // log data and check if transmission is required
	float ECVal = (float) ECSum / (float) reps; 
	//data.composeLine(ECVal, EC_TAG); //for SD card

	float tempVal = (float) tempSum / (float) reps; 
	//data.composeLine(tempVal, TEMP_TAG);

	double sensorPressure = (double) sensorPressureSum / (double) reps;
	//data.composeLine(sensorPressure, DEPTH_TAG);

	double airPressure = (double) airPressureSum / (double) reps;  //air pressure on the board
	//data.composeLine(airPressure, DEPTH_TAG);

	double pressure = sensorPressure - airPressure;  // true pressure in water
	// cable pressure - air pressure

	//data.writeLine(); //write the composed line to SD card

	Serial.print("EC = ");
	Serial.print(ECVal);
	Serial.print(", temp = ");
	Serial.print(tempVal);
	Serial.print(", pressure = ");
	Serial.println(sensorPressure);


	/// TRANSMIT CHECK ///
	// diff = (old - new) / old
	float ECdiff = (lastEC - ECVal) / lastEC;
	float tempDiff = (lastTemp - tempVal) / lastTemp;
	float pressureDiff = (lastPressure - pressure) / lastPressure;

	if (ECdiff >= 0.1 || tempDiff >= 0.1 || pressureDiff >= 0.1 || counter % 360 == 0) {
		transmit(ECVal, tempVal, pressure);
	}

	/// CLEAR SUMS FOR NEW MEASUREMENTS ///
	lastEC = ECVal;
	lastTemp = tempVal;
	lastPressure = pressure;

	ECSum = tempSum = airPressureSum = sensorPressureSum = 0; // clear all sums
	reps = 0;
}


///////////////////////////////////////////////////////////////
void transmit(float ECVal, float tempVal, double pressure) {
	/// turn on SIM7000 ///
	simOn();
  	bosl.begin(boslSS);

  	bosl.enableGPRS(true);

	char URL[150];

	char str_EC[10]; //convert floats to strings
	char str_temp[10];
	char str_pressure[10];
	dtostrf(ECVal, 2, 2, str_EC);
	dtostrf(tempVal, 2, 2, str_temp);
	dtostrf(pressure, 2, 2, str_pressure);

	sprintf(URL, "www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=%s&T=%s&EC=%s&D=%s", sitename, str_temp, str_EC, str_pressure); 

	if (!bosl.postData("GET", URL)) {
    	Serial.println(F("Failed to complete HTTP GET to EoDC database..."));
	}

	/// Power OFF ///
	simOff();
}


///////////////////////////////////////////////////////////////
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
