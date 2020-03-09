
//FFT library
#include "src\arduinoFFTfix.h"
#include "src\I2C.h"

#define PLOTFFT 0
#define LOWPRINT 1

//FFT sample buffers
int16_t read[128];
int16_t imag[128];

double max;
double avspeed;
double indx;
double rangeScaler;

double low = 0;
 
//Define FFT object
arduinoFFTfix FFTfix = arduinoFFTfix(); 

void setup(){

   I2c.begin();
   I2c.setSpeed(1); //Note 200 kHz Bus Speed
   Serial.begin(115200);
}

void printlow(){
	if (LOWPRINT){
	if (PLOTFFT){
		if (low){
			low = 0;
			Serial.println(-1);
		}
	}else{
		Serial.print("| ");
		Serial.print(low);
		Serial.print(", ");
	}		
	}
}

void loop(){
	double result;
    result = getVel(0,1);
		printlow();
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(result);
		Serial.print(" ");
		}
		
	result = getVel(1,1);
		printlow();
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(result);
		Serial.print(" ");
		}
	result = getVel(2,1);
		printlow();
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(result);
		Serial.print(" ");
		}
	result = getVel(3,1);
		printlow();
		if (PLOTFFT){
		plotFFT();
		}else{
		Serial.print(result);
		Serial.print(" ");
		}
	result = getVel(4,1);
		printlow();
		if (PLOTFFT){
			plotFFT();
			clearPlot();
		}else{
		Serial.println(result);
		}
    
}
// Plot FFT

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
		for(int i = 0; i < 128; i++){
		   imag[i] = 0;
	   }
	   
		//Compute FFT
		rangeScaler = FFTfix.RangeScaling(read, 128);
		FFTfix.Windowing(read, 128, FFT_FORWARD);
		FFTfix.Compute(read, imag, 128, FFT_FORWARD);
		FFTfix.ComplexToMagnitude(read, imag, 128);
			
			
		//nullRemove();	
		
				
		for(int i=2; i<(128/2); i++)
		{
			if (read[i] > max){
				max = read[i];
				indx = i;
			}
			
		}
		
		
		// if (max < 15){
			// low = 1;		
		// }
		/////////////////
		//betterMAX();
		
		//////////////////////
		delDCcompFFT();
		

		for(int i = 2; i < (128/2); i++){
			read[i] -=3;			
		}
		
		for(int i = 2; i < (128/2); i++){
		if (read[i] < 0){
			read[i] = 0;
		}
			
		}
		
		bool Npeak = 0;
		for(int i = indx; i < (128/2); i++){
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
		for(int i = 2; i < (128/2); i++){
			indx += read[i]*i;
			max += read[i];
		}
		indx = indx/max;
		
		low = max*rangeScaler;
		
		//converts frequency to mm/s
		//max = (indx*3.5)*velMulti;//MAX READING = 337 mm/s
		
		max = (indx)*(calArray[velMulti])*0.75;
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
	
	I2c.read(54,256,adcData);
	
	for(int i = 0; i <255; i += 2){
	int rsult = (adcData[i]-240)*256 + adcData[i+1];
	if (rsult > 2048){
		rsult = rsult - 4096;
	}
	read[i/2] = rsult;
	}
	
	//read[127] = read[126];
	
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
	for(int i=2; i<(128/2); i++)
		{
			read[i] -= (max - 5);
			if (read[i] < 0){
				read[i]= 0;
			}
		}
		max = 0;
		indx = 0;
		for(int i=1; i<(128/2); i++)
		{
			indx += read[i]*i;
			max +=read[i];	
			
		}
		indx = indx/max;
}

void plotRAW(){
	for(int i=0; i<(128); i++)
    {
        Serial.println(read[i]);   
    }
}

void plotFFT(){
	for(int i=2; i<(128/2); i++)
    {
        Serial.println(read[i], 1);   
    }

    for(int i=0; i<(1); i++)
   {
        Serial.println(-10);   
   }
    //plotRAW();
   // Serial.println("------------------------");
}
//TODO ADD GOOD AVERAGEING/DATA STUFF

//subtract null signal from fft data
void nullRemove(){
	for (int i =2; i<(128/2); i++){
		
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
	
	for( int i = 0; i < 128; i++){
		average += read[i];
	}
	average = average/128;
	
	for( int i = 0; i < 128; i++){
		read[i] -= average;
	}
}

void delDCcompFFT(){
	int32_t average = 0;
	
	for( int i = 2; i < (128/2); i++){
		average += read[i];
	}
	average = average/62;
	
	for( int i = 2; i < (128/2); i++){
		read[i] -= average;
	}
}



////////////////SPI INTERFACE///////////////

//SET Config
//SET Power
//Measure
//Return value
//sleep
