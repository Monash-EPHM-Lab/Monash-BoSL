#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

//FFT library
#include <arduinoFFT.h>

//FFT sample buffers
double read[128];
double imag[128];
 
//Define FFT object
arduinoFFT FFT = arduinoFFT(); 

void setup(){
    
  //set sampling rate to 17.6 kHz
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  
   Serial.begin(230400);
}

void loop(){
  double result;
  result = getVel(1,8);
  plotFFT();
  // Serial.print(result);
  // Serial.print(" ");
  result = getVel(2,8);
  plotFFT();
  // Serial.print(result);
  // Serial.print(" ");
  result = getVel(4,8);
  plotFFT();
  // Serial.print(result);
  // Serial.print(" ");
  result = getVel(8,8);
  plotFFT();
  // Serial.println(result);
    
}
// Plot FFT

double getVel(int velMulti, int averages){
	double max;
	double avspeed;
	double indx;
	
	
	avspeed = 0;
	
	for (int iter = 0; iter<averages; iter++){
		max = 0;
		indx = 0;
			
		
	   //sample 128 times at 2 kHz
	   for(int i = 0; i < 128; i++){
		   read[i] = analogRead(A1);
		   delayMicroseconds(2000/velMulti);
	   }

		//set imaginary componet of FFT to zero
		for(int i = 0; i < 128; i++){
		   imag[i] = 0;
	   }
	   
	   //Compute FFT
	   FFT.Windowing(read, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
	   FFT.Compute(read, imag, 128, FFT_FORWARD);
	   FFT.ComplexToMagnitude(read, imag, 128);
			
		for(int i=10; i<(128/2); i++)
		{
			if (read[i] > max){
				max = read[i];
				indx = i;
			}
			
		}
		
		for(int i=10; i<(128/2); i++)
		{
			read[i] -= (max - 5);
			if (read[i] < 0){
				read[i]= 0;
			}
		}
		max = 0
		indx = 0
		for(int i=10; i<(128/2); i++)
		{
			indx += read[i]*i;
			max +=read[i];	
			
		}
		indx = indx/max
		
		//converts frequency to mm/s
		max = (indx*3.5)*velMulti;//MAX READING = 337 mm/s
		avspeed += max;
	}
	avspeed = avspeed/averages;
    //Serial.println(avspeed/1000);
	return avspeed;
}

void plotFFT(){
	for(int i=10; i<(128/2); i++)
    {
        Serial.println(read[i], 1);   
    }

    for(int i=0; i<(436); i++)
   {
        Serial.println(0);   
   }
   delay(300);
}
//TODO ADD GOOD AVERAGEING/DATA STUFF

//subtract null signal from fft data
