
//FFT library
#include "src\arduinoFFTfix.h"


double source[128] = {0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1};

//FFT sample buffers
double read[128];
double imag[128];

 
//Define FFT object
arduinoFFTfix FFT = arduinoFFTfix(); 

void setup(){

   Serial.begin(115200);
}

void loop(){

		for(int i = 0; i < 128; i++){
			imag[i] = 0;
			read[i] = source[i];
		}
		
		//Compute FFT
		FFT.Windowing(read, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
		FFT.Compute(read, imag, 128, FFT_FORWARD);
		FFT.ComplexToMagnitude(read, imag, 128);

		plotFFT();
		clearPlot();
		
}


void plotFFT(){
	for(int i=2; i<(128/2); i++)
    {
        Serial.println(read[i], 1);   
    }
  
}


void clearPlot(){
	delay(3000);
	for(int i=0; i<500; i++)
   {
        Serial.println(0);   
   }
}
