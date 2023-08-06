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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "working.h"
#include "messages.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG			1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef enum{
	init = 0,
	run,
	stop,
	idle
}AppState;

AppState appState = init;
uint8_t rxByte = 0;
uint8_t rxBuff[10] = {0};
static float tSample;
uint32_t freqArray[N_POINTS] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LEDToggling(GPIO_TypeDef *LEDPort, uint16_t LEDPin, uint32_t mstime);
void LEDFixed(GPIO_TypeDef *LEDPort, uint16_t LEDPin);
static void resetSignal(Signal sig);

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

void LEDOff(GPIO_TypeDef *LEDPort, uint16_t LEDPin){
	HAL_GPIO_WritePin(LEDPort, LEDPin, GPIO_PIN_RESET);
}

static void resetSignal(Signal sig){
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

  Signal sig = {
  		.fFreq = 10, //Hz
		  .fCentral = 100, //kHz
		  .fRange = 5 //kHz
		  };

  float fullSin[N_POINTS]; //1024
  float sin[N_POINTS/4]; // 256
  float cos[N_POINTS/4]; //256


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
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  // Format the sin
  computeSinCos(sin, cos, N_POINTS);
  formatSin(fullSin, sin, cos);

  // Convert to PWM values
  convertToPWMlogic(freqArray, fullSin, sig, N_POINTS);

  // Set this refresh rate to TIM4 dedicated to sampling
  tSample = F_CLOCK/(N_POINTS * sig.fFreq); //20Hz -> 4101 --- 20kHz -> 4 refresh rate
  __HAL_TIM_SET_AUTORELOAD(&htim4, tSample);

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
			//__HAL_TIM_SET_AUTORELOAD(&htim3, freqArray[0]);
			//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, freqArray[0]/2);
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			HAL_TIM_Base_Start_IT(&htim4);
			appState = run;
			break;

		case run:
			LEDToggling(LD2_GPIO_Port, LD2_Pin, 200);
			break;

		case idle:
			LEDFixed(LD2_GPIO_Port, LD2_Pin);
			break;

		case stop:
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
			HAL_TIM_Base_Stop_IT(&htim4);
			appState = idle;
			break;
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

  if(GPIO_Pin != B1_Pin)
  	return;

  if(appState == run)
  	appState = stop;
  else if(appState == stop)
  	appState = run;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static uint8_t i = 0;

	if(htim->Instance != TIM4)
		return;

	// Update the ARR of TIM3 and his period to half way
	__HAL_TIM_SET_AUTORELOAD(&htim3, freqArray[i]);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, freqArray[i]/2);
	i++;

	if(i > (N_POINTS - 1))
		i = 0;

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

