/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "main.h"

#include "state_machine.h"

#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_tests.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

volatile uint32_t okButtonPressTime = 0;	// track the time the button has been pressed for
volatile uint32_t okButtonReleaseTime = 0;	// track the time the button has been released for
volatile uint32_t okButtonPressedDuration = 0;
volatile uint32_t okButtonReleasedDuration = 0;
volatile uint8_t okButtonState = 0;	// boolean to track button state, 0 is released, 1 is pressed
volatile uint8_t okDebounceFlag = 0;	// used to prevent debouncing

volatile uint32_t backButtonPressTime = 0;	// track the time the button has been pressed for
volatile uint32_t backButtonReleaseTime = 0;	// track the time the button has been released for
volatile uint32_t backButtonPressedDuration = 0;
volatile uint32_t backButtonReleasedDuration = 0;
volatile uint8_t backButtonState = 0;	// boolean to track button state, 0 is released, 1 is pressed
volatile uint8_t backDebounceFlag = 0;	// used to prevent debouncing

extern uint8_t okJustPressed;
extern uint8_t backJustPressed;
extern uint32_t timeArray[6];
extern uint8_t timerPaused;

extern uint32_t REFRESH_RATE;

extern uint32_t primedTimerCounter;
extern uint32_t primedDelay;
extern uint8_t primed;	// for tracking if the stopwatch is primed
extern uint8_t primedOutput;	// for tracking if the stopwatch output has already been done
extern uint8_t outputActive;

extern uint32_t outputTriggerTime;
extern uint32_t OUTPUT_TIME;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void update_time(){
//
//	if (time_array[0] > 1000){		// if past 1000 ms, increment seconds
//		time_array[0] = 0;
//		time_array[1]++;
//
//		if (time_array[1] >= 60){		// if past 60 seconds, increment minutes
//			time_array[1] = 0;
//			time_array[2]++;
//
//			if (time_array[2] >= 60){		// if past 60 minutes, increment hours
//				time_array[2] = 0;
//				time_array[3]++;
//
//				if (time_array[3] >= 24){		// if past 24 hours, increment days
//					time_array[3] = 0;
//					time_array[4]++;
//				}
//			}
//		}
//	}
//}


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
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

  uint32_t currentSec = 0;
  uint32_t lastPrint = 0;

  char msg1[] = "Hello World\r\n";
  HAL_UART_Transmit(&huart2, (const uint8_t*) msg1, strlen(msg1), HAL_MAX_DELAY);

  ssd1306_Init();
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  currentSec = time_array[1];
//	  update_time();
//	  ssd1306_TestAll();

	  // only print every 500 ms
	  if (HAL_GetTick() - lastPrint >= REFRESH_RATE){
		  lastPrint = HAL_GetTick();
		  updateState();
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

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
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 84-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 5000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 83;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 5000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 84-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 1000-1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ok_button_Pin back_button_Pin */
  GPIO_InitStruct.Pin = ok_button_Pin|back_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

// not going to use this function, will deal with it right in the ISR
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM9) {  // 500ms output timer

    	outputTriggerTime++;

    	if (outputTriggerTime >= OUTPUT_TIME){
            HAL_GPIO_WritePin(BUZZER_LED_GPIO_Port, BUZZER_LED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(HAPTIC_LED_GPIO_Port, HAPTIC_LED_Pin, GPIO_PIN_RESET);
            outputTriggerTime = 0;
            HAL_TIM_Base_Stop_IT(&htim9);
    	}

    }

	if (htim->Instance == TIM2){
		if (currentState == STOPWATCH_PRIMED && primed){
			primedTimerCounter++;
			if (primedTimerCounter >= primedDelay){
//				printf("exceed time \r\n");
                HAL_TIM_Base_Stop_IT(&htim2);
                currentState = STOPWATCH_RUN;
                primed = 0;

                primedOutput = 0;
                outputActive = 0;
//                printf("Stopwatch started!\r\n");
			}
		}
	}
    if (htim->Instance == TIM3)  // Check if TIM3 triggered the interrupt
    {
    	if (!timerPaused || currentState == STOPWATCH_RUN){  // only increment if timer is not paused
    		timeArray[0]++;
    	}

    	// adjust the time array
    	if (timeArray[0] > 1000){		// if past 1000 ms, increment seconds
    			timeArray[0] = 0;
//    			printf("increment counter\r\n");
    			timeArray[1]++;

    			if (timeArray[1] >= 60){		// if past 60 seconds, increment minutes
    				timeArray[1] = 0;
    				timeArray[2]++;

    				if (timeArray[2] >= 60){		// if past 60 minutes, increment hours
    					timeArray[2] = 0;
    					timeArray[3]++;

    					if (timeArray[3] >= 24){		// if past 24 hours, increment days
    						timeArray[3] = 0;
    						timeArray[4]++;

    						if (timeArray[4] >= 7){
    							timeArray[4] = 0;
    							timeArray[5]++;
    						}
    					}
    				}
    			}
    		}


    }

    if (htim->Instance == TIM5){
      	HAL_TIM_Base_Stop_IT(&htim5);	// stop debounce timer
		backDebounceFlag = 0;	// clear debounce flag
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET){	// if button still pressed
			if (backButtonState == 0){		// only register the press if there was a change in state (0 -> 1)

				backJustPressed = 1;

				backButtonPressTime = HAL_GetTick();
				backButtonState = 1;
				backButtonReleasedDuration = backButtonPressTime - backButtonReleaseTime;

//				char msg[60];
//				sprintf(msg, "back button Released for: %lu\r\n", backButtonReleasedDuration);
//				HAL_UART_Transmit(&huart2, (const uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
			}
		}

		else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET){		// if the button is released
			if (backButtonState == 1){	// only register the press if there was a change in state (0 -> 1)
				backButtonReleaseTime = HAL_GetTick();
				backButtonState = 0;
				backButtonPressedDuration = backButtonReleaseTime - backButtonPressTime;

//				char msg[60];
//				sprintf(msg, "back button Pressed for: %lu\r\n", backButtonPressedDuration);
//				HAL_UART_Transmit(&huart2, (const uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

			}
		}


    }

    if (htim->Instance == TIM4){

    	// debounce
    	HAL_TIM_Base_Stop_IT(&htim4);	// stop debounce timer
    	okDebounceFlag = 0;	// clear debounce flag
    	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET){	// if button still pressed
    		if (okButtonState == 0){		// only register the press if there was a change in state (0 -> 1)
    			okJustPressed = 1;
    			okButtonPressTime = HAL_GetTick();
				okButtonState = 1;
				okButtonReleasedDuration = okButtonPressTime - okButtonReleaseTime;


//				char msg[50];
//				sprintf(msg, "ok button Released for: %lu\r\n", okButtonReleasedDuration);
//				HAL_UART_Transmit(&huart2, (const uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
    		}
    	}

    	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET){		// if the button is released
    		if (okButtonState == 1){	// only register the press if there was a change in state (0 -> 1)

    			okButtonReleaseTime = HAL_GetTick();
    			okButtonState = 0;
    			okButtonPressedDuration = okButtonReleaseTime - okButtonPressTime;

//    			char msg[50];
//    			sprintf(msg, "ok button Pressed for: %lu\r\n", okButtonPressedDuration);
//    			HAL_UART_Transmit(&huart2, (const uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

    		}
    	}

    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if (GPIO_Pin == GPIO_PIN_4){

		if (okDebounceFlag == 0){
			okDebounceFlag = 1;
		}

		HAL_TIM_Base_Start_IT(&htim4);

//		char msg1[] = "Button Pressed\r\n";
//		HAL_UART_Transmit(&huart2, (const uint8_t*) msg1, strlen(msg1), HAL_MAX_DELAY);
	}

	if (GPIO_Pin == GPIO_PIN_6){
		if (backDebounceFlag == 0){
			backDebounceFlag = 1;
		}
		HAL_TIM_Base_Start_IT(&htim5);

	}


	// using the built in button
//	if (GPIO_Pin == GPIO_PIN_13){
//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//		char msg1[] = "Button Pressed\r\n";
//		HAL_UART_Transmit(&huart2, (const uint8_t*) msg1, strlen(msg1), HAL_MAX_DELAY);
//
//	}

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
