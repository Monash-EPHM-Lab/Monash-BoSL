#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

//FFT library
#include <arduinoFFT.h>

//FFT sample buffers
double read[128];
double imag[128];

double max;
int    indx;
    
//Define FFT object
arduinoFFT FFT = arduinoFFT(); 

void setup(){
    
  //set sampling rate to 17.6 kHz
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  //set up 40 kHz square wave on pin 3
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS21);
  OCR2A = 49;
  OCR2B = 24;
    
  
   Serial.begin(230400);
}

void loop(){
    
    max = 0;
    indx = 0; 
    
   //sample 128 times at 100 Hz
   for(int i = 0; i < 128; i++){
       read[i] = analogRead(A1);
       delay(10);
   }

    //set imaginary componet of FFT to zero
    for(int i = 0; i < 128; i++){
       imag[i] = 0;
   }
   
   //Compute FFT
   FFT.Windowing(read, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
   FFT.Compute(read, imag, 128, FFT_FORWARD);
   FFT.ComplexToMagnitude(read, imag, 128);
   
   
    for(int i=2; i<(128/2); i++)
    {
        if (max < read[i]){
            max = read[i];
            indx = i;
        }
        
    }
    
    //converts frequency to cm/s (for head to head)
    // devide by two for reflected
    max = 34*(indx*0.78)/40;
    
    //prints speed
    Serial.println(max);
      
   
   // //Plot FFT
   // for(int i=2; i<(128/2); i++)
    // {
        // Serial.println(read[i], 1);   
    // }

    // for(int i=0; i<(436); i++)
   // {
        // Serial.println(0);   
   // }
   
   // //delay for convenience
    // delay(3000);


   // //clear screen
      // for(int i=0; i<(512); i++)
   // {
        // Serial.println(0);   
   // }
    
}

