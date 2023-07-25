// Signal processing management
#include "working.h"

// Values for sin and cos computations
static const double a0 =  0.707107;
static const double a2 = -0.872348;
static const double a4 =  0.179252;
static const double a6 = -0.014272;
static const double b1 = -1.110670;
static const double b3 =  0.456159;
static const double b5 = -0.053910;

static const double pi =  3.141593;

static double normalizeOverPi(uint32_t x){
	return ((2/pi)*x - 1/2);
}

void computeSin(){


}


