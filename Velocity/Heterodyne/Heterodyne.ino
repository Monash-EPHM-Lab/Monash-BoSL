#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

//FFT library
#include <arduinoFFT.h>

//FFT sample buffers
double read[128];
double imag[128];

double max;
double avspeed;
double indx;
 
//Define FFT object
arduinoFFT FFT = arduinoFFT(); 

void setup(){
    
  //set sampling rate to 66 kHz
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
  
   Serial.begin(230400);
}

void loop(){
  double result;
    result = getVel(0,8);
    plotFFT();
//	Serial.print(result);
//	Serial.print(" ");
	result = getVel(1,8);
	plotFFT();
//	Serial.print(result);
//	Serial.print(" ");
	result = getVel(2,8);
	plotFFT();
//	Serial.print(result);
//	Serial.print(" ");
	result = getVel(3,8);
//	Serial.println(result);
	plotFFT();
	clearPlot();
    
}
// Plot FFT

double getVel(int velMulti, int averages){
	double calArray[4] = {3.70,7.35,14.28,26.55};
	int delayArray[4] = {2000,1000,500,250};
	
	avspeed = 0;
	
	for (int iter = 0; iter<averages; iter++){
		max = 0;
		indx = 0;
			
		
	   //sample 128 times at 2 kHz
	   for(int i = 0; i < 128; i++){
		   read[i] = analogRead(A1);
		   delayMicroseconds(delayArray[velMulti]);
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
		max = (indx)*(calArray[velMulti]);
		avspeed += max;
	}
	avspeed = avspeed/averages;
    //Serial.println(avspeed/1000);
	return avspeed;
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
		
		read[i] -= 385/(i*i);
	}	
}

void clearPlot(){
	delay(1500);
	for(int i=0; i<500; i++)
   {
        Serial.println(0);   
   }
}