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

void computeSinCos(double *sin, double *cos, const int nPoints){
	double t[nPoints];
	static double s;
	double tempSin[nPoints];
	double tempCos[nPoints];

	const double period = pi/2;
	formatTime(t, period, nPoints);
	for(int i=0; i<nPoints; i++){
		s = normalizeOverHalfPi(t[i]);
		tempSin[i] = a0+s*(-b1 + s*(a2 + s*(-b3 + s*(a4 + s*(-b5 + s*a6)))));
		tempCos[i] = a0+s*(b1+s*(a2 + s*(b3 + s*(a4 + s*(b5 + s*a6)))));
	}

	memcpy(sin, tempSin, sizeof(double) * nPoints);
	memcpy(cos, tempCos, sizeof(double) * nPoints);
}


