/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "working.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define F_CLOCK			84_000_000UL //84MHz
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef enum{
	init = 0,
	setup,
	run,
	stop,
	reset
}AppState;

typedef struct{
	//float fSampling;
	uint32_t fCentral; // in kHz
	uint32_t fRange; // in kHz
	uint32_t fFreq; // in Hz
} Signal;

AppState appState = init;
char txBuff[200];
static uint8_t flFirst = 1;
static double tSample;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LEDToggling(GPIO_TypeDef *LEDPort, uint16_t LEDPin, uint32_t mstime);
void resetSignal(Signal sig);
void LEDFixed(GPIO_TypeDef *LEDPort, uint16_t LEDPin);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LEDToggling(GPIO_TypeDef *LEDPort, uint16_t LEDPin, uint32_t mstime){
	static uint32_t sec = 0;
	uint32_t now;
	now = HAL_GetTick();

	if((now - sec) > mstime){ //toggle every mstime (expressed in ms)
		HAL_GPIO_TogglePin(LEDPort, LEDPin);
		sec = now;
	}
}

void LEDFixed(GPIO_TypeDef *LEDPort, uint16_t LEDPin){
	HAL_GPIO_WritePin(LEDPort, LEDPin, GPIO_PIN_SET);
}

void resetSignal(Signal sig){
	sig.fCentral = 0;
	sig.fRange = 0;
	sig.fFreq = 0;
	//sig.fSampling = 0;
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int periodFreqMin, periodFreqMax;
	float spaceSample;

  Signal sig = {
  		.fFreq = 0,
		  .fCentral = 0,
		  .fRange = 0
		  };

  resetSignal(sig);

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// FSM management
	switch(appState){
		case init:
			LEDToggling(LD2_GPIO_Port, LD2_Pin, 500);
			if(flFirst != 0){
				// Send UART message for fFreq

				flFirst = 0;
			}

			// Wait here for settings via UART
			//HAL_UART_Receive(&huart2, rxBuff, , )
			if(sig.fFreq == 0)
				break;

			// Send UART message for fCentral

			if(sig.fCentral == 0)
				break;

			// Send UART message for fRange

			if(sig.fRange == 0)
				break;


			break;
		case setup:
			// Setup
			periodFreqMax = F_CLOCK/(sig.fCentral + sig.fRange);
			periodFreqMin = F_CLOCK/(sig.fCentral - sig.fRange);

			// Setup samples for sampling timer
			tSample = 1/(N_POINTS * sig.fFreq);

			// Setup
			//spaceSample = (float)periodFreqMax/periodFreqMin;

			break;
		case run:
			LEDFixed(LD2_GPIO_Port, LD2_Pin);

			break;
		case stop:

			break;
		case reset:
			// Stop PWM output
			if(HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1) != HAL_OK){
				// handle error
			}

			// Reset signal
			resetSignal(sig);
			appState = init;
			break;
	    //case error:
		//	break;
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  static uint8_t prevState = 0;
  uint8_t newState;
  static uint32_t startTime = 0;
  uint32_t endTime;

  if(GPIO_Pin != B1_Pin)
	return;

  newState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

  if(newState == 1 && prevState == 0){
	startTime = HAL_GetTick();
  } else if(newState == 0 && prevState == 1) {
	endTime = HAL_GetTick();
	if((endTime - startTime) > 1000) //return to init
		appState = reset;
	else appState = (appState == stop) ? run:stop;
  }

  prevState = newState;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

