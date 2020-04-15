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

#include "arduinoFFTfix.h"

arduinoFFTfix::arduinoFFTfix(void)
{ // Constructor
	#warning("This method is deprecated and will be removed on future revisions.")
}


arduinoFFTfix::~arduinoFFTfix(void)
{
// Destructor
}

uint8_t arduinoFFTfix::Revision(void)
{
	return(FFT_LIB_REV);
}



void arduinoFFTfix::Compute(int16_t *vReal, int16_t *vImag, uint16_t samples, uint8_t dir)
{	
	uint8_t power = Exponent(samples);
	// Computes in-place complex-to-complex FFT
	// Reverse bits
	#warning("This method is deprecated and will be removed on future revisions.")
	uint16_t j = 0;
	for (uint16_t i = 0; i < (samples - 1); i++) {
		if (i < j) {
			Swap(&vReal[i], &vReal[j]);
			if(dir==FFT_REVERSE)
				Swap(&vImag[i], &vImag[j]);
		}
		uint16_t k = (samples >> 1);
		while (k <= j) {
			j -= k;
			k >>= 1;
		}
		j += k;
	}
	// Compute the FFT
	double c1 = -1.0;
	double c2 = 0.0;
	uint16_t l2 = 1;
	for (uint8_t l = 0; (l < power); l++) {
		uint16_t l1 = l2;
		l2 <<= 1;
		double u1 = 1.0;
		double u2 = 0.0;
		for (j = 0; j < l1; j++) {
			 for (uint16_t i = j; i < samples; i += l2) {
					uint16_t i1 = i + l1;
					double t1 = u1 * vReal[i1] - u2 * vImag[i1];
					double t2 = u1 * vImag[i1] + u2 * vReal[i1];
					vReal[i1] = (int16_t) (vReal[i] - t1);
					vImag[i1] = (int16_t) (vImag[i] - t2);
					vReal[i] = (int16_t) (vReal[i] + t1);
					vImag[i] = (int16_t) (vImag[i] +t2);
			 }
			 double z = ((u1 * c1) - (u2 * c2));
			 u2 = ((u1 * c2) + (u2 * c1));
			 u1 = z;
		}
		c2 = sqrt((1.0 - c1) / 2.0);
		if (dir == FFT_FORWARD) {
			c2 = -c2;
		}
		c1 = sqrt((1.0 + c1) / 2.0);
	}
}

float arduinoFFTfix::RangeScaling(int16_t *vReal, uint16_t samples)
{
	int max = 0;
	
	for(int i =0; i<(samples);i++){
		if (max < vReal[i]){
			max = vReal[i];
		}
		if (max < -vReal[i]){
			max = -vReal[i];
		}
	}
	float scaler = 256.0/max;
	
	for(int i =0; i<(samples);i++){
		vReal[i] = (int16_t) (vReal[i] * scaler);
	}
	return scaler;
}

void arduinoFFTfix::ComplexToMagnitude(int16_t *vReal, int16_t *vImag, uint16_t samples)
{	// vM is half the size of vReal and vImag
	#warning("This method is deprecated and will be removed on future revisions.")
	for (int16_t i = 0; i < samples; i++) {
		// Serial.print(i);
		// Serial.print(" ");
		// Serial.print(vReal[i]);
		// Serial.print(" ");
		// Serial.println(vImag[i]);
		vReal[i] = (int16_t) sqrt(sq((float)vReal[i]) + sq((float)vImag[i]));
	}
}

void arduinoFFTfix::Windowing(int16_t *vData, uint16_t samples, uint8_t dir)
{// Weighing factors are computed once before multiple use of FFT
// The weighing function is symetric; half the weighs are recorded
	#warning("This method is deprecated and will be removed on future revisions.")
	double samplesMinusOne = (double(samples) - 1.0);
	for (uint16_t i = 0; i < (samples >> 1); i++) {
		double indexMinusOne = double(i);
		double ratio = (indexMinusOne / samplesMinusOne);
		double weighingFactor = 1.0;
		// Compute and record weighting factor
		// hamming
			weighingFactor = 0.54 - (0.46 * cos(twoPi * ratio));
		if (dir == FFT_FORWARD) {
			vData[i] = (int16_t) vData[i]* weighingFactor;
			vData[samples - (i + 1)] = (int16_t) vData[samples - (i + 1)]*weighingFactor;
		}
		else {
			vData[i] = (int16_t) vData[i]/weighingFactor;
			vData[samples - (i + 1)] = (int16_t) vData[samples - (i + 1)]/weighingFactor;
		}
	}
}


uint8_t arduinoFFTfix::Exponent(int16_t value)
{
	#warning("This method will not be accessible on future revisions.")
	// Calculates the base 2 logarithm of a value
	uint8_t result = 0;
	while (((value >> result) & 1) != 1) result++;
	return(result);
}

// Private functions

void arduinoFFTfix::Swap(int16_t *x, int16_t *y)
{
	int16_t temp = *x;
	*x = *y;
	*y = temp;
}
