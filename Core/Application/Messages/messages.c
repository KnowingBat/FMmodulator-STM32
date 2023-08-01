#include "messages.h"

void messageRoutine(Signal *sig){
	static uint8_t state = 0;
	static uint32_t value = 0;
	switch(state){
		case 0: // Init & frequency message
			HAL_UART_Transmit(&huart2, msgInit, strlen(msgInit), UART_TIMEOUT);
			HAL_UART_Transmit(&huart2, msgFreq, strlen(msgFreq), UART_TIMEOUT);
			state++;
			break;
		case 1: //
			HAL_UART_Receive(&huart2, &value, 5, UART_TIMEOUT);
			if(value < MIN_FREQ || value > MAX_FREQ){
				if(value == 0)
				state = ; // Frequency inserted is wrong
				break;
			}
			state++;
			break;
		case 2:
			HAL_UART_Transmit(&huart2, msgCarr, strlen(msgCarr), UART_TIMEOUT);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;

	}


}

