/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "N64_Controls.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ZERO_VALUE_LOW            7
#define ZERO_VALUE_HIGH           20

#define ONE_VALUE_LOW             1
#define ONE_VALUE_HIGH            5

// The polling signal from the N64 is 7 zeros followed by 2 ones
#define ZERO_POLL_COUNT           7
#define ONE_POLL_COUNT            2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// The state defaults to high
uint8_t current_state = 1;

// Counter for how long the signal has been low
uint8_t low_count = 0;

// Counting how many zeros and ones the N64 has sent
uint8_t zero_count = 0;
uint8_t one_count = 0;

// Timestamp used for counting milliseconds
uint32_t timestamp = 0;

// Union containing struct/array of the N64 controller
N64_controller_u N64_controller;
N64_controller_u N64_controller_zero_status;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Enable timer 1 for microsecond delay
  HAL_TIM_Base_Start(&htim1);

  // Enable timer 3 for millisecond counting
  HAL_TIM_Base_Start_IT(&htim3);

  // Initialize controller status
  Update_Controller_Status(&N64_controller, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Read state of the data pin
	  current_state = HAL_GPIO_ReadPin(DATA_IN_GPIO_Port, DATA_IN_Pin);

	  if (current_state == 0) {
		  // Count while low
		  while (current_state == 0) {
			  low_count++;
			  current_state = HAL_GPIO_ReadPin(DATA_IN_GPIO_Port, DATA_IN_Pin);
		  }

		  // This value is a "zero" count
		  if (ZERO_VALUE_LOW < low_count && low_count < ZERO_VALUE_HIGH) {
			  zero_count++;
		  }

		  // This value is a "one" count
		  if (ONE_VALUE_LOW < low_count && low_count < ONE_VALUE_HIGH) {
			  one_count++;
		  }

		  // If we collect 7 zeros and 2 ones, polling signal is here
		  if (one_count == ONE_POLL_COUNT) {
			  if (zero_count == ZERO_POLL_COUNT) {
				  // If enough time has passed, respond with controller status
				  if (millisecond_counter - timestamp >= 2000) {
					  // Disable timer 3 global interrupt
					  HAL_TIM_Base_Stop_IT(&htim3);
					  // Respond with controller status here
					   Press_Buttons(&N64_controller);
					  // Start up timer interrupt
					  HAL_TIM_Base_Start_IT(&htim3);
					  timestamp = millisecond_counter;
				  } else { // Just send all zeros as if controller isn't doing anything
					  // Disable timer 3 global interrupt
					  HAL_TIM_Base_Stop_IT(&htim3);
					  // Respond with controller zero status here
					  Press_Buttons(&N64_controller_zero_status);
					  // Start up timer interrupt
					  HAL_TIM_Base_Start_IT(&htim3);
				  }
				  // Reset counts
				  zero_count = 0;
				  one_count = 0;
			  }
		  }

		  // If we don't have 7 zeros but get a one, reset counts
		  if (zero_count < 7) {
			  if (one_count > 0) {
				  zero_count = 0;
				  one_count = 0;
			  }
		  }

		  // If too many zeros come in, reset counts
		  if (zero_count > 7) {
			  zero_count = 0;
			  one_count = 0;
		  }

		  // If too many ones come in, reset counts
		  if (one_count > 2) {
			  zero_count = 0;
			  one_count = 0;
		  }

		  // Reset low counter
		  low_count = 0;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
