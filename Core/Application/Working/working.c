// Signal processing management
#include "working.h"
#include <string.h>


// Values for sin and cos computations
static const double a0 =  0.707107;
static const double a2 = -0.872348;
static const double a4 =  0.179252;
static const double a6 = -0.014272;
static const double b1 = -1.110670;
static const double b3 =  0.456159;
static const double b5 = -0.053910;

static const double pi =  3.141593;

static double normalizeOverHalfPi(double x){
	return ((2/pi)*x - 1/2);
}

static void formatTime(double *tArray, const double period, const int nPoints){
	//@TODO: check if the size is equivalent to nPoints...
	// if not, return something...
	double sampleSpace = (double)(period/nPoints);
	for(int i=0; i<nPoints-1; i++){
		tArray[i+1] = tArray[i]+sampleSpace;
	}
}

static void negateArray(double *arrayToNegate, const double *array, uint32_t size){
	double tempArray[size];
	memcpy(tempArray, array, sizeof(double)*size);

	for(int i=0; i<size; i++){
		if(tempArray[i] == 0)
			continue;

		tempArray[i] = -tempArray[i];
	}

	memcpy(arrayToNegate, tempArray, sizeof(double) * size);
}

void computeSinCos(double *sin, double *cos, const int nPoints){
	const normNPoints = nPoints/4; // i need "nPoints" points over a period, but here i compute only a quarter of a period
	double t[normNPoints];
	static double s;
	double tempSin[normNPoints];
	double tempCos[normNPoints];
	const double period = pi/2;
    // Build the time array from 0 to pi/2
	formatTime(t, period, normNPoints);
	for(int i=0; i<normNPoints; i++){
		s = normalizeOverHalfPi(t[i]);
		tempSin[i] = a0+s*(-b1 + s*(a2 + s*(-b3 + s*(a4 + s*(-b5 + s*a6)))));
		tempCos[i] = a0+s*(b1+s*(a2 + s*(b3 + s*(a4 + s*(b5 + s*a6)))));
	}

	memcpy(sin, tempSin, sizeof(double) * normNPoints);
	memcpy(cos, tempCos, sizeof(double) * normNPoints);
}

void formatSin(double *signalToFormat, double *sin, double *cos){
	double nSin[N_POINTS/4];
	double nCos[N_POINTS/4];

	for(int i=0; i<4; i++){
		if(i == 0){
			// First quadrant
			memcpy(signalToFormat, sin, sizeof(double) * N_POINTS/4);
			signalToFormat += sizeof(double) * N_POINTS/4;
		}

		if(i == 1){
			// Second quadrant
			memcpy(signalToFormat, cos, sizeof(double) * N_POINTS/4);
			signalToFormat += sizeof(double) * N_POINTS/4;
		}

		if(i == 2){
			// Third quadrant
			negateArray(nSin, sin, N_POINTS/4);
			memcpy(signalToFormat, nSin, sizeof(double) * N_POINTS/4);
			signalToFormat += sizeof(double) * N_POINTS/4;
		}

		if(i == 3){
			// Forth quadrant
			negateArray(nCos, cos, N_POINTS/4);
			memcpy(signalToFormat, nCos, sizeof(double) * N_POINTS/4);
		}
	}
}

void convertToPWMlogic(uint32_t *freqArray, const double *fullSin, Signal sig, uint32_t size){
	double tempFreq = 0;
	for(int i=0;i<size;i++){
		tempFreq = sig.fCentral + fullSin[i] * sig.fRange;
		// Now transform it to the ARR value for timer
		freqArray[i] = F_CLOCK/tempFreq;
	}
}




