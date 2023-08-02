#include "messages.h"
#include "usart.h"
#include <string.h>

const char *msgInit = "FM Modulator\n";
const char *msgFreq = "Insert the signal frequency (in Hz): ";
const char *msgCarr = "Insert the carrier central frequency (in kHz): ";
const char *msgRAnge = "Insert the frequency range(in kHz): ";

void messageRoutine(Signal *sig){
	static uint8_t state = 0;
	static uint32_t value = 0;
	switch(state){
		case 0: // Init & frequency message
			HAL_UART_Transmit(&huart2, (uint8_t *)msgInit, strlen(msgInit), UART_TIMEOUT);
			HAL_UART_Transmit(&huart2, (uint8_t *)msgFreq, strlen(msgFreq), UART_TIMEOUT);
			//state++;
			break;
		case 1: //
			HAL_UART_Receive(&huart2, (uint8_t *)&value, 5, UART_TIMEOUT);
			if(value < MIN_FREQ || value > MAX_FREQ){
				if(value == 0)
				state = 3; // Frequency inserted is wrong
				break;
			}
			state++;
			break;
		case 2:
			HAL_UART_Transmit(&huart2, (uint8_t *)msgCarr, strlen(msgCarr), UART_TIMEOUT);
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

