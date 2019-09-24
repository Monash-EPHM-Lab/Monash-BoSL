#include <Wire.h>
#include <SparkFun_MS5803_I2C.h> // Click here to get the library
#include <LowPower.h> // Click here to get the library
#include <SPI.h>
#include <SdFat.h>
#include <NeoSWSerial.h>
#include "SensorEC.h"
#include "SIM7000.h"

#define BAUDRATE 9600 // MUST be below 19200 (for stability) but 9600 is more stable
#define MAX_URL_SIZE 150 // URL (with sensor values) greater than (or close to) this will cause issues

// For SIM7000 BoSL board
#define BOSL_TX 2 // Microcontroller TX
#define BOSL_RX 3 // Microcontroller RX
#define PWRKEY 4
#define DTR 5 // Connect with solder jumper
#define CS 10 //chip select

NeoSWSerial boslSS(BOSL_RX, BOSL_TX);
SIM7000 bosl = SIM7000(); 

//  ADDRESS_HIGH = 0x76 // ADDRESS_LOW  = 0x77
MS5803 boardSensor(ADDRESS_LOW);
MS5803 cableSensor(ADDRESS_HIGH);

const uint8_t ECPinR = A0;  //EC read pin
const uint8_t ECPinA = A1;  //EC +- pin
const uint8_t ECPinB = A3;  //EC +- pin
const uint8_t MOSIPin = 11; //for SD card 
const uint8_t MISOPin = 12; //SD card reserved
const uint8_t SCKPin = 13;  //SD card
const uint8_t CSPin = 10;   //SD card


const __FlashStringHelper* URL;   // Special strings to save RAM
const __FlashStringHelper* sitename;


char filename[22] = "DD-MM-YY hh_mm_ss.txt";  // default file and format. if changing must change newFilename
uint16_t counter = 0;

uint8_t reps = 0;
float lastTemp, lastEC;
double lastPressure;  // last true pressure reading. sensor pressure - air pressure
float tempSum = 0, ECSum = 0;
double sensorPressureSum = 0, airPressureSum = 0;
uint16_t batLevel = 4200; //mV

SensorEC EC = SensorEC(ECPinR, ECPinA, ECPinB);  //three pins therefore switching is true.


// slightly more complicated but saves 20% of available RAM
void initialiseStrings() {  // we initialise strings in flash using some special functions.
	URL = F("www.cartridgerefills.com.au/EoDC/databases/WriteMe.php?SiteName=%s&EC=%s&T=%s&D=%s&B=%i");
	//URL to transmit to with placeholders for values (lookup printf syntax)
	sitename = F("BoSLv02_longTest.csv");  // sitename 
}

///////////////////////////////////////////////////////////////
void setup() {
	initialiseStrings();

  	Serial.begin(BAUDRATE);
  	Serial.println(F("BoSL Initialising"));

	Wire.begin();
  	// initialise both sensors
  	cableSensor.reset();
	boardSensor.reset();
  	cableSensor.begin();
  	boardSensor.begin();

	pinMode(PWRKEY, OUTPUT);
  	pinMode(DTR, OUTPUT);

    pinMode(CSPin, OUTPUT);  //SD card
    pinMode(MOSIPin, OUTPUT);
    pinMode(MISOPin, INPUT);
    pinMode(SCKPin, OUTPUT);


  	simOn(); // See function definition at the very end of the sketch
 	// ensure that board is set to 9600 baudrate from 115200 default. 
  	//boslSS.begin(115200); // Default SIM7000 shield baud rate
 	//  Serial.println(F("Configuring to 9600 baud"));
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

  	// Configure a GPRS APN.
  	bosl.setNetworkSettings(F("telstra.internet")); // For Telstra (Australia) SIM card - CAT-M1 (Band 28)
  	//bosl.setNetworkSettings(F("yesinternet")); // For optus card
  	//bosl.setNetworkSettings(F("mdata.net.au")); // For aldi card

  	bosl.setPreferredLTEMode(1); // Use LTE CAT-M only, not NB-IoT
  	bosl.setOperatingBand("CAT-M", 28); // Telstra uses band 28

  	//newFilename();  //new file on startup

	bosl.flush(); // must run before going to sleep
  	simOff();
}


///////////////////////////////////////////////////////////////
void loop() {
	Serial.print(F("count: "));
	Serial.println(counter);

	Serial.flush(); // ensures that all messages have sent through serial before arduino sleeps

	LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  //check to make sure its actually 1min
	LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);  

	counter++;  // increment these in loop only
	reps++; //this must come before logData();

	ECSum += EC.measure(); //take measurement every 10sec
	tempSum += cableSensor.getTemperature(CELSIUS, ADC_512);
	sensorPressureSum += cableSensor.getPressure(ADC_4096);	
	airPressureSum += boardSensor.getPressure(ADC_4096);	

	if (reps == 6 || counter % 8640 == 0) { //one minute has passed or a new day
		logAndTransmitCheck();
	}
}


///////////////////////////////////////////////////////////////
void logAndTransmitCheck() {  // log data and check if transmission is required
	ECSum = (float) ECSum / (float) reps; 
	tempSum = (float) tempSum / (float) reps; 
	sensorPressureSum = (double) sensorPressureSum / (double) reps;
	airPressureSum = (double) airPressureSum / (double) reps;  //air pressure on the board
	double pressure = sensorPressureSum - airPressureSum;  // true pressure in water
	// redefine sums to equal their average value (sum/reps) to save memory by not requiring new variables.

	Serial.print(F("EC="));
	Serial.print(ECSum);
	Serial.print(F(",temp="));
	Serial.print(tempSum);
	Serial.print(F(",sensorPressure="));
	Serial.print(sensorPressureSum);
	Serial.print(F(",airPressure="));
	Serial.println(airPressureSum);

	saveToSD();  // log data to SD card

	/// TRANSMIT CHECK ///
	// diff = (old - new) / old
	float ECdiff = (lastEC - ECSum) / lastEC;
	float tempDiff = (lastTemp - tempSum) / lastTemp;
	float pressureDiff = (lastPressure - pressure) / lastPressure;

  //counter = 360 after an hour
	if (ECdiff >= 0.1 || tempDiff >= 0.1 || pressureDiff >= 0.1 || counter % 360 == 0) {
		transmit(pressure);
	}

	/// CLEAR SUMS FOR NEW MEASUREMENTS ///
	lastEC = ECSum;
	lastTemp = tempSum;
	lastPressure = pressure;

	ECSum = tempSum = airPressureSum = sensorPressureSum = 0; // clear all sums
	reps = 0;
}


///////////////////////////////////////////////////////////////
void transmit(double pressure) {
/// turn on SIM7000 ///
  	simOn();

  	boslSS.begin(BAUDRATE);
  	bosl.begin(boslSS);

	bosl.enableGPRS(true);
  	delay(5);  //read battery voltage after start up
	bosl.getBattVoltage(&batLevel);

	char URL_buffer[MAX_URL_SIZE];
	char str_EC[10]; //convert floats to strings
	char str_temp[10];
	char str_pressure[10];
	char str_sitename[40];  // make sure this is big enough to hold the sitename initialised at beginning
	dtostrf(ECSum, 2, 2, str_EC);
	dtostrf(tempSum, 2, 2, str_temp);
	dtostrf(pressure, 2, 2, str_pressure);
	strcpy_P(str_sitename, (PGM_P)sitename); // copy sitename from flash to RAM. this is deallocated after function ends

	sprintf_P(URL_buffer, (PGM_P)URL, str_sitename, str_EC, str_temp, str_pressure, batLevel); 
	// use _P and cast to (PGM_P) to use flash string

	if (counter % 8640 == 0) {  // if it is a new day change files
		newFilename();
	}

	if (!bosl.postData("GET", URL_buffer)) {
  		Serial.println(F("Failed to complete HTTP GET to EoDC database..."));
	}

	/// Power OFF ///
	simOff();
}


///////////////////////////////////////////////////////////////
void simOn() {
	pinMode(PWRKEY, OUTPUT);  // important pinmodes
	pinMode(BOSL_TX, OUTPUT);
	digitalWrite(BOSL_TX, HIGH);
	pinMode(BOSL_RX, INPUT_PULLUP);

	digitalWrite(PWRKEY, LOW);  //pulse pin for power on
	delay(100); // For SIM7000
	digitalWrite(PWRKEY, HIGH);
}


///////////////////////////////////////////////////////////////
void simOff() {
	//  TX / RX pins off to save power
	digitalWrite(BOSL_TX, LOW);
	digitalWrite(BOSL_RX, LOW);

	digitalWrite(PWRKEY, LOW);
	delay(3000); // For SIM7000
	digitalWrite(PWRKEY, HIGH);
}


///////////////////////////////////////////////////////////////
////// SD CARD FUNCTIONS //////
void dump(void) {  //dump array
    Serial.print(F("----SENSOR DATA---- Reading file: "));
    Serial.println(filename);

    SdFat sd;
    SdFile file;

    if (!sd.begin(10, SD_SCK_MHZ(8))) {
        sd.initErrorHalt();
    }  // initialise sd card

    file.open(filename, O_RDONLY);

    char c;
    while (file.available()) {
        c = file.read();
        Serial.print(c);
    }

    file.close();
}


///////////////////////////////////////////////////////////////
void saveToSD() {
	Serial.print(F("saving to file: "));
	Serial.println(filename);

    SdFat sd;
    SdFile file;

    if (!sd.begin(CS, SD_SCK_MHZ(4))) {
        sd.initErrorHalt();
    }  // initialise sd card

    file.open(filename, O_WRONLY | O_CREAT | O_AT_END);

    file.print(F("count="));
    file.print(counter);
    file.print(F(",EC="));
    file.print(ECSum);
    file.print(F(",temp="));
    file.print(tempSum);
    file.print(F(",cablePressure="));
    file.print(sensorPressureSum);
    file.print(F(",airPressure="));
    file.print(airPressureSum);
    file.print(F(",bat="));
    file.println(batLevel);
    file.close();

	//set SD card to sleep
 	SPI.end(); 
  	digitalWrite(SCKPin, HIGH);
  	digitalWrite(CS, HIGH);
  	digitalWrite(MOSIPin, HIGH);

  	Serial.println(F("saved"));
}  // memory for sd and file are freed after function ends


void newFilename() {
	char time[21]; // reply from SIM module (in format: "YY/MM/DD,hh:mm:ss)
	// for example response could be: "19/08/25,20:01:27
  	bosl.getTime(time, 19);  // 19 chars contain the date and time 

  	//filename format as follows: DD-MM-YY hh_mm_ss (done char by char) 
  	filename[0] = time[7]; // day
  	filename[1] = time[8]; // day
  	//filename[2] stays the same "-"
  	filename[3] = time[4]; //month
  	filename[4] = time[5]; //month
  	//filename[5] stays the same "-"
  	filename[6] = time[1]; //year
  	filename[7] = time[2]; //year
  	//filename[8] stays the same " "
  	filename[9] = time[10]; //hour
  	filename[10] = time[11]; //hour
  	//filename[11] stays the same "_"
  	filename[12] = time[13]; //minute
  	filename[13] = time[14]; //minute
  	//filename[14] stays the same " "
  	filename[15] = time[16]; //second
  	filename[16] = time[17]; //second
  	// filename chars 17-21 is ".txt"
}