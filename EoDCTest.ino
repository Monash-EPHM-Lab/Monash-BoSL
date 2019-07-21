#include <SensorData.h>
#include <EC.h>


/////////////////////////////
// tags for storing and retreiving data in CSV file on SD card
#define TIME_TAG "t="
#define DEPTH_TAG "depth="
#define TEMP_TAG "temp="
#define EC_TAG "EC="
#define BAT_TAG "bat="
/////////////////////////////

#define logHistory 60  // max number of readings/columns to be transmitted

#define BAUDRATE 9600

const int ECPinR = A0;
const int ECPinA = A1;
const int ECPinB = A3;

const int diffThreshold = 0.1;  //10% change in consecutive readings before transmitting

uint16_t counter = 0;


SensorEC EC = SensorEC(ECPinR, ECPinA, ECPinB, true);  //three pins and switching is true.

SensorData data = SensorData("startfile.txt", logHistory); //initialise data array


///////////////////////////////////////////////////////////////
void setup() {
	Serial.begin(BAUDRATE);
	data.dump();
}


///////////////////////////////////////////////////////////////
void loop() {
	delay(1000);
	//delay(10000);
	counter++;

	EC.measure(); //take measurement every 10sec
/*	temp.measure();
	depth.measure();	*/

	if (counter % 6 == 0) { //one minute has passed
		logData();
		//transmitCheck();
	}
}


///////////////////////////////////////////////////////////////
void logData() { // save all data from each sensor

	float ECVal = EC.getAverage(); 
	data.composeLine(ECVal, EC_TAG);
	EC.clearSum();   //clear the EC sensor value to start a new average.

/*	float tempVal = temp.getAverage(); 
	data.composeLine(tempVal, TEMP_TAG);
	temp.clearSum();   

	float depthVal = depth.getAverage(); 
	data.composeLine(depthVal, DEPTH_TAG);
	depth.clearSum();    */

	data.writeLine(); //write the composed line to SD card
}


///////////////////////////////////////////////////////////////
void transmitCheck() {

	float ECdiff = (EC.getLast() - EC.getAverage()) / EC.getLast(); 
	// EC percentage change: (old - new) / old


/*	float tempDiff = (temp.getLast() - temp.getAverage()) / temp.getLast(); 

	float depthDiff = (depth.getLast() - depth.getAverage()) / depth.getLast(); */

//	if (ECdiff >= diffThreshold || tempDiff >= diffThreshold || depthDiff >= diffThreshold) {
	if (ECdiff >= diffThreshold) {
		transmit(); //make this do something....
	}
} 
