#define I2CADR 0b0110110
#include "src\I2C.h"

// int read[512];
uint8_t a[1024];
	
	
void setup(){
	I2c.begin();
	I2c.write(54, 0b11011100);
	I2c.write(54, 0b00000010);
	Serial.begin(230400);
}

void loop(){
	
	I2c.read(54,256,a);
	
	for(int i=0; i<(254); i += 2)
    {
		int rsult = (a[i]-240)*256 + a[i+1];
		if (rsult > 2048){
			rsult = rsult - 4096;
		}
        Serial.println(rsult); 
		
    }
	
	
	// for(int i =0; i<1024; i += 2){
		// I2c.read(54, 2);
		
		// a[i] = I2c.receive();
		// a[i+1] = I2c.receive();
		
	// }
	
	// for(int i=0; i<(1024); i += 2)
    // {
        // Serial.println((a[i]-240)*256 + a[i+1]);   
    // }
	
	// for(int i=0; i<(500-256); i++){
		// Serial.println(0);
	// }
	delay(400);
	for(int i=0; i<500;i++){
		Serial.println(0);
	}
}