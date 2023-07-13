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
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bno055.h"
#include "user_define.h"
#include "lcd.h"
#include "stdio.h"
#include "mpc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
uint8_t tmp, i,j,member;
char status[4];
char data1[120];
char data2[120];
char data3[120];
char data4[120];
float temp,roll,pitch,yaw;
float lia_x,lia_y,lia_z;
float gyr_x,gyr_y,gyr_z;
float angular_acc_x,angular_acc_y,angular_acc_z;
char sys,gyr,acc,mag;
float ax,ay,az,gx,gy,gz;
uint8_t Rxbuffer[20];
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_CAN2_Init();
  MX_I2C1_Init();
  //MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  //MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  //MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM5_Init();
  HAL_TIM_Base_Start(&TIM_DELAY_US);
  /* USER CODE BEGIN 2 */
  SSD1306_Init (); // initialize the display

  SSD1306_GotoXY (10,10); // goto 10, 10
  SSD1306_Puts ("HELLO", &Font_16x26, 1); // print Hello
  SSD1306_GotoXY (10, 38);
  SSD1306_Puts ("WORLD!!", &Font_7x10, 1);
  SSD1306_UpdateScreen(); // update screen
  delay_ms(1000);

  SSD1306_Clear();
  SSD1306_DrawBitmap(0, 0,drone_logo,128, 64, 1);
  SSD1306_UpdateScreen();
  delay_ms(2000);

  /* Infinite loop */
  bno055_initization();
  delay_ms(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  bno055_calibrations_status(&sys,&gyr,&acc,&mag);
	  bno055_get_elu_data(&roll, &pitch, &yaw);
	  bno055_get_temp(&temp);
	  bno055_get_accel_gyro(&ax,&ay,&az,&gx,&gy,&gz);
	  delay_ms(50);
	  bno055_get_accel_gyro(&ax,&ay,&az,&gyr_x,&gyr_x,&gyr_x);
	  angular_acc_x = (gyr_x - gx ) / 50;
	  angular_acc_y = (gyr_y - gy ) / 50;
	  angular_acc_z = (gyr_z - gz ) / 50;
	  bno055_get_lia_data(&lia_x, &lia_y, &lia_z);
	  //HAL_UART_Transmit(&huart6,(uint8_t *)data1, sprintf(data1,"%0.5f,%0.5f,%0.5f,%0.5f,%0.5f,%0.5f,%0.5f\n",roll,pitch,yaw,temp,lia_x,lia_y,lia_z),10);
	  //HAL_UART_Transmit(&huart6,(uint8_t *)data1, sprintf(data1,"%0.5f,%0.5f,%0.5f\n",gx,gy,gz),10);
	  HAL_UART_Transmit(&huart6,(uint8_t *)data2, sprintf(data2,"%0.5f,%0.5f,%0.5f\n",angular_acc_x,angular_acc_y,angular_acc_z),10);
	  //sprintf(data2,"%c,%c,%c,%c\n",sys,gyr,acc,mag);
	  //HAL_UART_Transmit(&huart6,(uint8_t *)data2,sprintf(data2,"%d,%d,%d,%d\n",sys,gyr,acc,mag),10);
	  //HAL_UART_Transmit(&huart6,(uint8_t *)data3,sprintf(data3,"%0.5f,%0.5f,%0.5f,%0.5f,%0.5f,%0.5f\n",ax,ay,az,gx,gy,gz),10);
	  delay_ms(100);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
