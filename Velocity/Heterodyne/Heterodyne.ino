#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

//FFT library
#include <arduinoFFT.h>

//FFT sample buffers
double read[128];
double imag[128];

double max;
double avspeed;
int    indx;
    
//Define FFT object
arduinoFFT FFT = arduinoFFT(); 

void setup(){
    
  //set sampling rate to 17.6 kHz
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  // //set up 40 kHz square wave on pin 3
  // pinMode(3, OUTPUT);
  // pinMode(11, OUTPUT);
  // TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  // TCCR2B = _BV(WGM22) | _BV(CS20);
  // OCR2A = 15;
  // OCR2B = 7;
    
  
   Serial.begin(230400);
}

void loop(){
   
	avspeed = 0;
	for (int iter = 0; iter<4; iter++){
    max = 0;
    indx = 0;
		
    
   //sample 128 times at 100 Hz
   for(int i = 0; i < 128; i++){
       read[i] = analogRead(A1);
       delayMicroseconds(1000);
   }

    //set imaginary componet of FFT to zero
    for(int i = 0; i < 128; i++){
       imag[i] = 0;
   }
   
   //Compute FFT
   FFT.Windowing(read, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
   FFT.Compute(read, imag, 128, FFT_FORWARD);
   FFT.ComplexToMagnitude(read, imag, 128);
   
   for(int i = 0; i<128/2; i++){
	   read[i] -= 5;
	   if (read[i] < 0){
		   read[i] = 0;
	   }
   }
   
    for(int i=7; i<(128/2); i++)
    {
        indx += read[i]*i;
		max += read[i];
    }
		indx = indx/max;
    
    //converts frequency to mm/s (for head to head)
    // devide by two for reflected
    max = (indx*7.14)*0.75;//MAX READING = 337 mm/s
    avspeed += max;
    // //prints speed
	}
	avspeed = avspeed/4;
   // Serial.println(avspeed);
      
   
  // Plot FFT
   // for(int i=7; i<(128/2); i++)
    // {
        // Serial.println(read[i], 1);   
    // }

    // for(int i=0; i<(436); i++)
   // {
        // Serial.println(0);   
   // }
   
   //delay for convenience
    //delay(500);


   // // // //clear screen
      // // // for(int i=0; i<(512); i++)
   // // // {
        // // // Serial.println(0);   
   // // // }
    
}


//TODO ADD GOOD AVERAGEING/DATA STUFF

//subtract null signal from fft data
