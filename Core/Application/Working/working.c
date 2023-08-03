// Signal processing management
#include "working.h"
#include <string.h>


// Values for sin and cos computations
static const float a0 =  0.707107;
static const float a2 = -0.872348;
static const float a4 =  0.179252;
static const float a6 = -0.014272;
static const float b1 = -1.110670;
static const float b3 =  0.456159;
static const float b5 = -0.053910;

static const float pi =  3.141593;

static float normalizeOverHalfPi(float x){
	float retVal = (float)-1/2 + 2*x/pi;
	return retVal;
}

static void formatTime(float *tArray, const float period, const int nPoints){
	//@TODO: check if the size is equivalent to nPoints...
	// if not, return something...
	tArray[0] = 0;
	float sampleSpace = (float)(period/(nPoints-1));
	for(int i=0; i<nPoints-1; i++){
		tArray[i+1] = tArray[i]+sampleSpace;
	}
}

static void negateArray(float *arrayToNegate, const float *array, uint32_t size){
	float tempArray[size];
	memcpy(tempArray, array, sizeof(float)*size);

	for(int i=0; i<size; i++){
		if(tempArray[i] == 0)
			continue;

		tempArray[i] = -tempArray[i];
	}

	memcpy(arrayToNegate, tempArray, sizeof(float) * size);
}

void computeSinCos(float *sin, float *cos, const int nPoints){
	const uint16_t normNPoints = nPoints/4; // i need "nPoints" points over a period, but here i compute only a quarter of a period
	float t[normNPoints];
	static float s;
	float tempSin[normNPoints];
	float tempCos[normNPoints];
	const float period = pi/2;
    // Build the time array from 0 to pi/2
	formatTime(t, period, normNPoints);
	for(int i=0; i<normNPoints; i++){
		s = normalizeOverHalfPi(t[i]);
		tempSin[i] = a0+s*(-b1 + s*(a2 + s*(-b3 + s*(a4 + s*(-b5 + s*a6)))));
		tempCos[i] = a0+s*(b1+s*(a2 + s*(b3 + s*(a4 + s*(b5 + s*a6)))));
	}

	memcpy(sin, tempSin, sizeof(float) * normNPoints);
	memcpy(cos, tempCos, sizeof(float) * normNPoints);
}

void formatSin(float *signalToFormat, float *sin, float *cos){
	float nSin[N_POINTS/4];
	float nCos[N_POINTS/4];
	uint32_t size = N_POINTS/4;

	for(int i=0; i<4; i++){
		if(i == 0){
			// First quadrant
			memcpy(&signalToFormat[i*size], sin, sizeof(float) * size);
		}

		if(i == 1){
			// Second quadrant
			memcpy(&signalToFormat[i*size], cos, sizeof(float) * size);
		}

		if(i == 2){
			// Third quadrant
			negateArray(nSin, sin, size);
			memcpy(&signalToFormat[i*size], nSin, sizeof(float) * size);
		}

		if(i == 3){
			// Forth quadrant
			negateArray(nCos, cos, size);
			memcpy(&signalToFormat[i*size], nCos, sizeof(float) * size);
		}
	}
}

void convertToPWMlogic(uint32_t *freqArray, const float *fullSin, Signal sig, uint32_t size){
	float tempFreq = 0;
	for(int i=0;i<size;i++){
		tempFreq = sig.fCentral + fullSin[i] * sig.fRange;
		// Now transform it to the ARR value for timer
		freqArray[i] = F_CLOCK/tempFreq;
	}
}




