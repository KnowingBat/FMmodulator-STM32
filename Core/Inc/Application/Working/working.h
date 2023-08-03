#include "main.h"

#define N_POINTS 		1024

// Functions prototypes
void computeSinCos(float *sin, float *cos, const int nPoints);
void formatSin(float *signalToFormat, float *sin, float *cos);
void convertToPWMlogic(uint32_t *freqArray, const float *fullSin, Signal sig, uint32_t size);



