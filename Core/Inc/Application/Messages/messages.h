#include "main.h"

#define UART_TIMEOUT	 1000
#define MIN_FREQ         20
#define MAX_FREQ			20000

void messageRoutine(Signal *sig, uint8_t *rxBuff);
