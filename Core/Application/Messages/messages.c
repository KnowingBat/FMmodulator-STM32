#include "messages.h"
#include "usart.h"
#include <string.h>

const char *msgInit = "FM Modulator\n";
const char *msgFreq = "Insert the signal frequency (in Hz): ";
const char *msgCarr = "Insert the carrier central frequency (in kHz): ";
const char *msgRange = "Insert the frequency range(in kHz): ";

void messageRoutine(Signal *sig){
	static uint8_t state = 0;
	uint8_t rxBuff[4] = {0};
	uint32_t value = 0;
	uint32_t valMult = 1;

	switch(state){
		case 0: // Init & frequency message
			HAL_UART_Transmit(&huart2, (uint8_t *)msgInit, strlen(msgInit), UART_TIMEOUT);
			HAL_UART_Transmit(&huart2, (uint8_t *)msgFreq, strlen(msgFreq), UART_TIMEOUT);
			state++;
			break;
		case 1: //
			if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE)){
				HAL_UART_Receive(&huart2, (uint8_t *)&rxBuff, 4, UART_TIMEOUT);
			}

			if(rxBuff[0] != 0 || rxBuff[1] != 0 || rxBuff[2] != 0 || rxBuff[3] != 0)
			{
				for(int i=0;i<4;i++){
					value += (rxBuff[i]-48)*valMult;
					valMult *= 10;
				}
			}

			if(value != 0){
				if(value < MIN_FREQ || value > MAX_FREQ){
					state = 6; // Wrong value inserted, go to...
				}
				sig->fFreq = value;
				state++;
			}
			break;
		case 2:
			HAL_UART_Transmit(&huart2, (uint8_t *)msgCarr, strlen(msgCarr), UART_TIMEOUT);
			state++;
			break;
		case 3:
			HAL_UART_Receive(&huart2, (uint8_t *)&value, 4, UART_TIMEOUT);
			if(value != 0){
				if(value < MIN_FREQ || value > MAX_FREQ){
					state = 7; // Wrong value inserted, go to...
				}
				sig->fCentral = value;
				state++;
			}
		  break;
		case 4:
			HAL_UART_Transmit(&huart2, (uint8_t *)msgRange, strlen(msgRange), UART_TIMEOUT);
			state++;
			break;
		case 5:
			HAL_UART_Receive(&huart2, (uint8_t *)&value, 4, UART_TIMEOUT);
			if(value != 0){
				if(value < MIN_FREQ || value > MAX_FREQ){
					state = 8; // Wrong value inserted, go to...
				}
				sig->fRange = value;
				state++;
			}
			break;
		case 6:
			break;
		case 7:
			break;

	}


}

