/*

	FFT libray
	Copyright (C) 2010 Didier Longueville
	Copyright (C) 2014 Enrique Condes

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef arduinoFFTfix_h /* Prevent loading library twice */
#define arduinoFFTfix_h
#ifdef ARDUINO
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h" /* This is where the standard Arduino code lies */
	#endif
#else
	#include <stdlib.h>
	#include <stdio.h>
	#ifdef __AVR__
		#include <avr/io.h>
	#endif
	#include <math.h>
	#include "defsfix.h"
	#include "typesfix.h"
#endif

#define FFT_LIB_REV 0x14
/* Custom constants */
#define FFT_FORWARD 0x01
#define FFT_REVERSE 0x00

/*Mathematial constants*/
#define twoPi 6.28318531
#define fourPi 12.56637061
#define sixPi 18.84955593

class arduinoFFTfix {
public:
	/* Constructor */
	arduinoFFTfix(void);
	/* Destructor */
	~arduinoFFTfix(void);
	/* Functions */
	uint8_t Revision(void);
	uint8_t Exponent(int16_t value);
	void ComplexToMagnitude(int16_t *vReal, int16_t *vImag, uint16_t samples);
	void Compute(int16_t *vReal, int16_t *vImag, uint16_t samples, uint8_t dir);
	void Windowing(int16_t *vData, uint16_t samples, uint8_t dir);
	float RangeScaling(int16_t *vReal, uint16_t samples);


private:
	/* Variables */
	uint16_t _samples;
	double _samplingFrequency;
	int16_t *_vReal;
	int16_t *_vImag;
	uint8_t _power;
	/* Functions */
	void Swap(int16_t *x, int16_t *y);
};

#endif
