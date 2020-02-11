
//FHT library
#include <fix_fft.h>
#include "src\I2C.h"


int16_t f[128];

void setup(){
   I2c.begin();
   Serial.begin(230400);
}

void loop(){
	
	sampleFast();

	   
	   fix_fftr(f, 128, 0);
	   
	   for(int i=4; i<(128/2); i++)
    {
        Serial.println(f[i]);   
    }

		for(int i=0; i<(65); i++)
	   {
			Serial.println(0);   
	   }
//	plotFFT();
	clearPlot();
    
}
// Plot FFT

void getVel(int velMulti, int averages){

	   
}


void sampleFast(){
	uint8_t adcData[256];
	
	I2c.write(54, 0b11011100);
	I2c.write(54, 0b00000010);
	
	I2c.read(54,255,adcData);
	
	for(int i = 0; i <254; i += 2){
	int rsult = (adcData[i]-240)*256 + adcData[i+1];
	if (rsult > 2048){
		rsult = rsult - 4096;
	}
	f[i/2] = 8*rsult;
	}
	
	int rsult = (adcData[254]-240)*256 + adcData[253];
	if (rsult > 2048){
		rsult = rsult - 4096;
	}
	f[127] = 8*rsult;
	
}



void plotFFT(){
	
}
//TODO ADD GOOD AVERAGEING/DATA STUFF


void clearPlot(){
	delay(1500);
	for(int i=0; i<500; i++)
   {
        Serial.println(0);   
   }
}