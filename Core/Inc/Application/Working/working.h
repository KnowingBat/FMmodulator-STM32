#include "main.h"

#define N_POINTS 		1024

// Functions prototypes
void computeSinCos(double *sin, double *cos, const int nPoints);
void formatSin(double *signalToFormat, double *sin, double *cos);
void convertToPWMlogic(uint32_t *freqArray, const double *fullSin, Signal sig, uint32_t size);



