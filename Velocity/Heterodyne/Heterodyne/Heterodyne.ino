
//FFT library
#include <arduinoFFT.h>
#include "src\I2C.h"

//FFT sample buffers
double read[128];
double imag[128];

double max;
double avspeed;
double indx;
 
//Define FFT object
arduinoFFT FFT = arduinoFFT(); 

void setup(){
    


   I2c.begin();
   Serial.begin(230400);
}

void loop(){
  double result;
    result = getVel(0,4);
//    plotFFT();
	Serial.print(result);
	Serial.print(" ");
	result = getVel(1,4);
//	plotFFT();
	Serial.print(result);
	Serial.print(" ");
	result = getVel(2,4);
//	plotFFT();
	Serial.print(result);
	Serial.print(" ");
	result = getVel(3,4);
	Serial.println(result);
//	plotFFT();
//	clearPlot();
    
}
// Plot FFT

double getVel(int velMulti, int averages){
	double calArray[4] = {4.09,6.99,10.9,41.3};
	
	avspeed = 0;
	
	for (int iter = 0; iter<averages; iter++){
		max = 0;
		indx = 0;
			
	   if (velMulti == 3){
	   sampleFast();
	   }
	   if (velMulti == 2){
		sampleSlow(400);
	   }
	   if (velMulti == 1){
		sampleSlow(800);
	   }
	   if (velMulti == 0){
		sampleSlow(1600);
	   }



		//set imaginary componet of FFT to zero
		for(int i = 0; i < 128; i++){
		   imag[i] = 0;
	   }
	   
	   //Compute FFT
	   FFT.Windowing(read, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
	   FFT.Compute(read, imag, 128, FFT_FORWARD);
	   FFT.ComplexToMagnitude(read, imag, 128);
			
			
		nullRemove();	
			
		//add null condition for if amplitude is small	
		for(int i=4; i<(128/2); i++)
		{
			if (read[i] > max){
				max = read[i];
				indx = i;
			}
			
		}
		
		
		/////////////////
		//betterMAX();
		
		//////////////////////
		
		//converts frequency to mm/s
		//max = (indx*3.5)*velMulti;//MAX READING = 337 mm/s
		max = (indx)*(calArray[velMulti]);//*0.75;
		avspeed += max;
	}
	avspeed = avspeed/averages;
    //Serial.println(avspeed/1000);
	return avspeed;
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
	read[i/2] = rsult;
	}
	
	int rsult = (adcData[254]-240)*256 + adcData[253];
	if (rsult > 2048){
		rsult = rsult - 4096;
	}
	read[127] = rsult;
	
}

void sampleSlow(int delay){
	I2c.write(54, 0b11010100);
	I2c.write(54, 0b00000010);
	
	uint8_t MSB;
	uint8_t LSB;
	
	
	for(int i =0; i<256; i += 2){
		I2c.read(54, 2);
		
		MSB = I2c.receive();
		LSB = I2c.receive();
		
		int rsult = (MSB-240)*256 + LSB;
		if (rsult > 2048){
			rsult = rsult - 4096;
		}
		read[i/2] = rsult;

		delayMicroseconds(delay);
	}
	
	
	
}

void betterMAX(){
	for(int i=4; i<(128/2); i++)
		{
			read[i] -= (max - 5);
			if (read[i] < 0){
				read[i]= 0;
			}
		}
		max = 0;
		indx = 0;
		for(int i=3; i<(128/2); i++)
		{
			indx += read[i]*i;
			max +=read[i];	
			
		}
		indx = indx/max;
}

void plotRAW(){
	for(int i=0; i<(128); i++)
    {
        Serial.println(read[i], 1);   
    }
}

void plotFFT(){
	for(int i=4; i<(128/2); i++)
    {
        Serial.println(read[i], 1);   
    }
//436
    for(int i=0; i<(65); i++)
   {
        Serial.println(0);   
   }
}
//TODO ADD GOOD AVERAGEING/DATA STUFF

//subtract null signal from fft data
void nullRemove(){
	for (int i =4; i<(128/2); i++){
		
		read[i] -= 1000/(i*i);
	}	
}

void clearPlot(){
	delay(1500);
	for(int i=0; i<500; i++)
   {
        Serial.println(0);   
   }
}