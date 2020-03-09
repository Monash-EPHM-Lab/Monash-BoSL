
//FFT library
#include "src\arduinoFFTfix.h"
#include <arduinoFFT.h>

//FFT sample buffers
int16_t readfix[128];
int16_t imagfix[128];

double read[128];
double imag[128];
 
float scaler;
 
//Define FFT object
arduinoFFTfix FFTfix = arduinoFFTfix(); 
arduinoFFT FFT = arduinoFFT(); 

void setup(){

   Serial.begin(115200);
}

void loop(){
		scaler = 0;
		 
		for(int i = 0; i < 128; i++){
			imag[i] = 0;
			read[i] = random(-8,8);
			imagfix[i] = 0;
			readfix[i] = (int16_t)read[i];
		}

			
		//Compute FFT
		scaler = FFTfix.RangeScaling(readfix, 128);
				
		FFTfix.Windowing(readfix, 128, FFT_FORWARD);
		FFTfix.Compute(readfix, imagfix, 128, FFT_FORWARD);
		FFTfix.ComplexToMagnitude(readfix, imagfix, 128);
		
		FFT.Windowing(read, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
		FFT.Compute(read, imag, 128, FFT_FORWARD);
		FFT.ComplexToMagnitude(read, imag, 128);

		plotFFT();
		//clearPlot();
		
}


void plotFFT(){
	float maxread = 0;
	for(int i =0; i<(128/2);i++){
		if (maxread < read[i]){
			maxread = read[i];
		}
			
	}
	
	for(int i=0; i<(128/2); i++)
    {
		Serial.println(((read[i]*scaler - readfix[i])*100)/maxread);
		// Serial.print(" ");
		// Serial.print(readfix[i]);
		// Serial.print(" ");
		// Serial.println(read[i]*scaler);   
    }
  
}


void clearPlot(){
	delay(3000);
	for(int i=0; i<500; i++)
   {
		Serial.print(0);
		Serial.print(" ");
		Serial.print(0);
		Serial.print(" ");
        Serial.println(0);   
   }
}
