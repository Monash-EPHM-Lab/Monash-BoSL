
//FOR USE WITH 8 MHz ATMEGA328P//

#include "Sleeps.h"

#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <Arduino.h>
//millis timer variable 
extern volatile unsigned long timer0_millis;


//note only good for long delays
void SleepClass::extDelay(uint32_t tmz){
    clock_prescale_set(clock_div_256);
    delay(tmz/256);
    clock_prescale_set(clock_div_1);
    cli();
    timer0_millis += tmz;
    sei();
    
}

void SleepClass::sleeps(uint16_t tmz){
    
    ISR(TIMER2_COMPA_vect){
    TCCR2B  = 0b00000000; 
    }

    clock_prescale_set(clock_div_256);
    
    tmz = tmz/8;
    
    while (tmz > 0){
    
        timer2on();
        set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
        sleep_mode(); 
        
        cli();
        timer0_millis += 8388;
        sei();
        
        tmz -= 1;
    }
    
    clock_prescale_set(clock_div_1);
}

void SleepClass::timer2on(void){
cli();
TCCR2A  = 0b00000010; //see docs for register func
TCCR2B  = 0b00000111; //see docs for register func
OCR2A   = 0b11111111; //top of timer
TIMSK2 |= 0b00000010; //set interput high
sei();     
}

//define interrupt vector

