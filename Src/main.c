/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t buf[1];
uint8_t print_buf[600];
uint32_t print_num;
uint8_t line[64];
uint32_t line_cnt;

uint8_t opcode[2];
uint8_t command;
uint8_t option[2];

uint8_t main_load[4];
uint8_t matching_network[11];
uint8_t rc_load[4];
uint8_t rf_band[3];

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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  memset(buf,0,sizeof(buf));
  memset(print_buf,0,sizeof(print_buf));
  memset(line,0,sizeof(line));
  line_cnt = 0;
  memset(opcode,0,sizeof(opcode));
  memset(option,0,sizeof(option));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	HAL_UART_Receive(&huart3, (uint8_t*)buf, 1, HAL_MAX_DELAY);
	switch (buf[0]) {
		case  '\r':
//			HAL_UART_Transmit(&huart3,"\n\r",2, HAL_MAX_DELAY);
//			HAL_UART_Transmit(&huart3,(uint8_t*)line,line_cnt, HAL_MAX_DELAY);
//			HAL_UART_Transmit(&huart3,"\n\r",2, HAL_MAX_DELAY);

			strncpy(opcode,line,2);
			command = line[3];
			strncpy(option,&line[5],3);


			if (!strncmp(opcode, "LD", 2)){
				//HAL_UART_Transmit(&huart3,"\n\rMAIN LOAD\n\r",13, HAL_MAX_DELAY);
				switch(command){
					case 'G':
						print_num = sprintf(print_buf,"\n\rMain Load: %s\n\r",main_load);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					case 'S':
						memset(main_load,0,sizeof(main_load));
						strncpy(main_load,&line[5],3);
						/*
						 * Send proper GPIO signal:
						 * Pins: PA3, PC0, PC3
						 */
						if (!strncmp(main_load, "OPN", 3)){ 		// OPEN => 000
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
						}
						else if (!strncmp(main_load, "SHT", 3)){	// SHORT => 001
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
						}
						else if (!strncmp(main_load, "25", 2)){		// 25ohm => 010
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
						}
						else if (!strncmp(main_load, "50", 2)){		// 50ohm => 011
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
						}
						else if (!strncmp(main_load, "200", 3)){	// 200ohm => 100
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
						}
						else if (!strncmp(main_load, "CTR", 3)){	// CONNECTOR => 101
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
						}
						else if (!strncmp(main_load, "50j", 3)){	// 50-j => 110
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
						}
						else if (!strncmp(main_load, "MNT", 3)){	// MATCHING NETWORK => 111
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
						}
						else {
							print_num = sprintf(print_buf,
									"\n\r***Unrecognized load parameter***"
									"\n\rAcceptable: OPN, SHT, 25, 50, 200, CTR, 50j, MNT"
									"\n\rType '??' for further description"
									);
							HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
							memset(print_buf,0,sizeof(print_buf));
						}
						HAL_UART_Transmit(&huart3,"\n\r",2, HAL_MAX_DELAY);
						break;
					case 'H':
						print_num = sprintf(print_buf,
								"\n\rAcceptable commands for 'LD':"
								"\n\rG (get), S (set), H (help)"
								"\n\rFollow (S)et command with parameter selection (OPN,SHT,25,50,200,CTR,50j,MNT)"
								"\n\rType '??' for further detailed help\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					default:
						print_num = sprintf(print_buf,
								"\n\rUnrecognized command for opcode"
								"\n\rType 'LD H' for help\n\r");
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
				}
			}
			else if (!strncmp(opcode, "MN", 2)){
				//HAL_UART_Transmit(&huart3,"\n\rMATCHING NETWORK\n\r",20, HAL_MAX_DELAY);
				switch(command){
					case 'G':
						print_num = sprintf(print_buf,"\n\rMatching Network: %s\n\r",matching_network);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					case 'S':
						break;
					case 'H':
						print_num = sprintf(print_buf,
								"\n\rAcceptable commands for 'MN':"
								"\n\rG (get), S (set), H (help)"
								"\n\r **setting parameters tbd**"
								"\n\rType '??' for further detailed descriptions\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					default:
						print_num = sprintf(print_buf,
								"\n\rUnrecognized command for opcode"
								"\n\rType 'MN H' for help\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
				}
			}
			else if(!strncmp(opcode, "RC", 2)){
				//HAL_UART_Transmit(&huart3,"\n\rRC LOAD\n\r",11, HAL_MAX_DELAY);
				switch(command){
					case 'G':
						print_num = sprintf(print_buf,"\n\rRC Load: %s\n\r",rc_load);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					case 'S':
						break;
					case 'H':
						print_num = sprintf(print_buf,
								"\n\rAcceptable commands for 'RC':"
								"\n\rG (get), S (set), H (help)"
								"\n\rFollow (S)et command with load number selection (LD1,LD2,LD3,etc.)"
								"\n\rType '??' for further detailed descriptions\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					default:
						print_num = sprintf(print_buf,
								"\n\rUnrecognized command for opcode"
								"\n\rType 'RC H' for help\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
				}
			}
			else if(!strncmp(opcode, "RF", 2)){
				//HAL_UART_Transmit(&huart3,"\n\rRF LOAD\n\r",11, HAL_MAX_DELAY);
				switch(command){
					case 'G':
						print_num = sprintf(print_buf,"\n\rRF Band: %s\n\r",rf_band);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					case 'S':
						break;
					case 'H':
						print_num = sprintf(print_buf,
								"\n\rAcceptable commands for 'RF':"
								"\n\rG (get), S (set), H (help)"
								"\n\rFollow (S)et command with band selection* (BD1,BD2,BD3,etc.)"
								"\n\r*v1.0-only one band available"
								"\n\rType '??' for further detailed help\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
					default:
						print_num = sprintf(print_buf,
								"\n\rUnrecognized command for opcode"
								"\n\rType 'RF H' for help\n\r"
								);
						HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
						memset(print_buf,0,sizeof(print_buf));
						break;
				}
			}
			else if(!strncmp(opcode, "??", 2)){
				//HAL_UART_Transmit(&huart3,"\n\rHELP VERBOSE\n\r",16, HAL_MAX_DELAY);
				print_num = sprintf(print_buf,
						"\n\r<OPCODE> <COMMAND> <PARAMETERS>"
						"\n\r"
						"\n\rAcceptable OPCODEs:"
						"\n\rLD - main load selection for system"
						"\n\rMN - matching network selection based off desired VSWR"
						"\n\rRC - RC plasma load selection"
						"\n\rRF - system's radio frequency band selection"
						"\n\r"
						"\n\rAcceptable COMMANDs per opcode:"
						"\n\rG - (G)et current opcode setting"
						"\n\rS - (S)et to a desired parameter"
						"\n\rH - display (H)elp instructions for current opcode use"
						"\n\r"
						"\n\rMain Load PARAMETERS:"
						"\n\rOPN (open), SHT (short), 25, 50, 200 (resistances in 'ohms' respectively)"
						"\n\rCTR (connector), 50j (50-j impedance), MNT (matching network)\n\r"
						);
				HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
				memset(print_buf,0,sizeof(print_buf));
			}
			else if(!strncmp(opcode, "?", 1)){
				//HAL_UART_Transmit(&huart3,"\n\rHELP\n\r",8, HAL_MAX_DELAY);
				print_num = sprintf(print_buf,
						"\n\rAcceptable opcodes:"
						"\n\rLD (main load),	MN (matching network), RC (RC load), RF (RF band)"
						"\n\rAcceptable commands per opcode:"
						"\n\rG (get), S (set), H (help)"
						"\n\rType '??' for verbose help\n\r"
						);
				HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
				memset(print_buf,0,sizeof(print_buf));
			}
			else {
				print_num = sprintf(print_buf,
						"\n\rUNRECOGNIZED OPCODE"
						"\n\rType '?' for help, or '??' for verbose help\n\r"
						);
				HAL_UART_Transmit(&huart3,print_buf,print_num, HAL_MAX_DELAY);
				memset(print_buf,0,sizeof(print_buf));
			}
			memset(line,0,sizeof(line));
			line_cnt = 0;
			break;
		default:
			HAL_UART_Transmit(&huart3, (uint8_t*)buf, 1, HAL_MAX_DELAY);
			if (buf[0] == '\177'){
				line_cnt--;
				memset(&line[line_cnt],0,1);
			}
			else {
				strncat(line,buf,1);
				line_cnt++;
			}
			break;
	}
	memset(buf,0,sizeof(buf));
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
