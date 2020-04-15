
//FFT library
#include "src\arduinoFFTfix.h"
#include "src\I2C.h"
#include <LowPower.h>

#define PLOTFFT 0
#define LOWPRINT 1
#define SAMPLES 256
#define ADCADR 0x34
#define INTPIN 2

//FFT sample buffers
int16_t read[SAMPLES];
int16_t imag[SAMPLES];

double max;
double avspeed;
double indx;
double rangeScaler;

double low = 0;
 
//Define FFT object
arduinoFFTfix FFTfix = arduinoFFTfix(); 



void setup(){
	
   Serial.begin(9600);
   Serial.print('R');
     
}

void printlow(){
	if (LOWPRINT){
	if (PLOTFFT){
		if (low){
			low = 0;
			Serial.println(-1);
		}
	}else{
		Serial.print(", ");
		Serial.print(low/1000,0);
	}		
	}
}

void loop(){
	uint8_t cmd;
	if (Serial.available() > 0){
		cmd = Serial.read();
		
		if(cmd == 'V'){
			printVel()
		}
		if(cmd == 'S'){
			sleepF();
		}
		
		
	}
	
}

void sleepF(){
	LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

// Plot FFT
void printVel(){
	pinMode(3,OUTPUT);
	pinMode(9,OUTPUT);
	digitalWrite(9,HIGH);
	digitalWrite(3,HIGH);
	
    I2c.begin();
    I2c.setSpeed(1); //Note 200 kHz Bus Speed
	
	
	
	
	
	double result;
    result = getVel(0,3);
		
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(result,0);
		}
		printlow();
		
	result = getVel(1,3);
		
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(", ");
		Serial.print(result,0);
		
		}
		printlow();
		
	result = getVel(2,3);
		
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(", ");
		Serial.print(result,0);
		}
		printlow();
		
	result = getVel(3,3);
		
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(", ");
		Serial.print(result,0);
		}
		printlow();
		
	result = getVel(4,3);
		if (PLOTFFT){
			plotFFT();
			//clearPlot();
		}else{
		Serial.print(", ");
		Serial.print(result,0);
		}
		printlow();
		
	digitalWrite(9,LOW);
	digitalWrite(3,LOW);	
	I2c.end();	
	
	Serial.print('T');
		
}


double getVel(int velMulti, int averages){
	double calArray[5] = {5.08,10.2,21.0,40.7,80.6};
	
	avspeed = 0;
	rangeScaler = 0;
	
	for (int iter = 0; iter<averages; iter++){
		
		
		max = 0;
		indx = 0;
			
	   if (velMulti == 4){
	   sampleFast();
	   }
	   if (velMulti == 3){
		sampleSlow(20);
	   }
	   if (velMulti == 2){
		sampleSlow(200);
	   }
	   if (velMulti == 1){
		sampleSlow(600);
	   }
	   if (velMulti == 0){
		sampleSlow(1400);
	   }

		//delete DC component of signal
		delDCcomp();

		//set imaginary componet of FFT to zero
		for(int i = 0; i < SAMPLES; i++){
		   imag[i] = 0;
	   }
	   
		// //Compute FFT
		rangeScaler = FFTfix.RangeScaling(read, SAMPLES);
		FFTfix.Windowing(read, SAMPLES, FFT_FORWARD);
		FFTfix.Compute(read, imag, SAMPLES, FFT_FORWARD);
		FFTfix.ComplexToMagnitude(read, imag, SAMPLES);
			
			
		//////////nullRemove();	
		
				
		for(int i=2; i<(SAMPLES/2); i++)
		{
			if (read[i] > max){
				max = read[i];
				indx = i;
			}
			
		}
		
		
		delDCcompFFT();
		

		for(int i = 2; i < (SAMPLES/2); i++){
			read[i] -=3; //MAJIK number			
		}
		
		for(int i = 2; i < (SAMPLES/2); i++){
		if (read[i] < 0){
			read[i] = 0;
		}
			
		}
		
		bool Npeak = 0;
		for(int i = indx; i < (SAMPLES/2); i++){
			if (read[i] == 0){
				Npeak = 1;
			}
			if (Npeak){
				read[i] = 0;
			}
		}
		Npeak = 0;
		for(int i = indx; i > 0; i--){
			if (read[i] == 0){
				Npeak = 1;
			}
			if (Npeak){
				read[i] = 0;
			}
		}
		indx = 0;
		max = 0;
		for(int i = 2; i < (SAMPLES/2); i++){
			indx += read[i]*(float)i;
			max += read[i];
		}
		indx = indx/max;
		
		low = max*rangeScaler/(SAMPLES/128);
		
		//converts frequency to mm/s
		//max = (indx*3.5)*velMulti;//MAX READING = 337 mm/s
		
		max = (indx)*(calArray[velMulti])*0.75/(SAMPLES/128);
		
		avspeed += max;
	}
	avspeed = avspeed/averages;
    //Serial.println(avspeed/1000);
	return avspeed;
}


void sampleFast(){
	uint8_t adcData[SAMPLES*2];
	
	I2c.write(ADCADR, 0b10101100);
	I2c.write(ADCADR, 0b00000010);
	
	I2c.readex(ADCADR,SAMPLES*2,adcData);
	
	for(int i = 0; i <SAMPLES*2; i += 2){
	int rsult = (adcData[i]-240)*256 + adcData[i+1];
	if (rsult > 2048){
		rsult = rsult - 4096;
	}
	read[i/2] = rsult;
	}
	
}

void sampleSlow(int delay){
	I2c.write(ADCADR, 0b10100100);
	I2c.write(ADCADR, 0b00000010);
	
	uint8_t MSB;
	uint8_t LSB;
	
	
	for(int i =0; i<SAMPLES*2; i += 2){
		I2c.read(ADCADR, 2);
		
		MSB = I2c.receive();
		LSB = I2c.receive();

		int16_t rsult = (MSB-240)*256 + LSB;
		if (rsult > 2048){
			rsult = rsult - 4096;
		}
		read[i/2] = rsult;
		delayMicroseconds(delay);
	}
	
	
	
}


void plotRAW(){
	for(int i=0; i<(SAMPLES); i++)
    {
        Serial.println(read[i]);   
    }
}

void plotFFT(){
	for(int i=2; i<(SAMPLES/2); i++)
    {
        Serial.println(read[i]);   
    }

    for(int i=0; i<(1); i++)
   {
        Serial.println(-10);   
   }

}
//TODO ADD GOOD AVERAGEING/DATA STUFF

//subtract null signal from fft data
void nullRemove(){
	for (int i =2; i<(SAMPLES/2); i++){
		
		read[i] -= 1000/(i*i);
	}	
}

void clearPlot(){
	delay(3000);
	for(int i=0; i<500; i++)
   {
        Serial.println(0);   
   }
}

void delDCcomp(){
	int32_t average = 0;
	
	for( int16_t i = 0; i < SAMPLES; i++){
		average += read[i];
	}
	average = (average/SAMPLES);
	
	for( int16_t i = 0; i < SAMPLES; i++){
		read[i] -= average;
	}
}

void delDCcompFFT(){
	int32_t average = 0;
	
	for( int i = 2; i < (SAMPLES/2); i++){
		average += read[i];
	}
	average = average/((SAMPLES/2)-2);
	
	for( int i = 2; i < (SAMPLES/2); i++){
		read[i] -= average;
	}
}



////////////////SPI INTERFACE///////////////

//SET Config
//SET Power
//Measure
//Return value
//sleep
