#include <SensorData.h>
#include <EC.h>


/////////////////////////////
// Sensor indexes for data array
#define _Time 0
#define _Pressue 1
#define _Temp 2
#define _EC 3
#define _Bat 4 
/////////////////////////////

#define sensorNum 5    // number of sensors/rows to be in data array
#define logHistory 10  // number of readings/columns to be kept in data array

#define BAUDRATE 9600

const int ECPinR = 10;
const int ECPinA = 11;
const int ECPinB = 12;

const int diffThreshold = 0.1;  //10% change in consecutive readings before transmitting

uint16_t counter = 0;


EC sensorEC = EC(ECPinR, ECPinA, ECPinB, true);  //three pins and switching is true.


void setup() {


  Serial.begin(BAUDRATE);

  SensorData data = SensorData(sensorNum, logHistory); //initialise data array
  data.print();
}

// the loop function runs over and over again forever
void loop() {
	delay(1000000);
	//delay(10000);
	counter++;
	sensorEC.measure();

	if (counter % 6 == 0) {
		//logData();
		//transmitCheck();
	}
}

/*
float getDiff(int sensor) {
	float newReading = data.getLastFloat(sensor);
	float oldReading = data.get2ndLastFloat(sensor); //can change to not be most recent reading.

	float diff = (oldReading - newReading) / oldReading;  

	return diff;
}

void logData(){
	float ECVal = sensorEC.getAverage(true); //clear the EC sensor value to start a new average.
	data.saveNew(ECVal, _EC);
}

void transmitCheck() {
	float ECDiff = getDiff(_EC);

	if (ECDiff >= diffThreshold) {
		//transmit(); //make this do something....
	}

} */

