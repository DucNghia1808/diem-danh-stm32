/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "i2c-lcd.h"
#include "cJSON.h"
//#include "usart.h"
#include "R305.h"
#include "flash.h"
#include "ringbuf.h"
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
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

//UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define ADDRESS_DATA_STORAGE	0x800FC00
RINGBUF RX_BUF;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

// truyen printf
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else 
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
	
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
void nutnhan();
void quetphim(char *ten);


void send_data_uart(uint8_t Mode, uint8_t Admin, uint8_t Id, uint8_t Name);
void send_data_uart1(uint8_t Mode, uint8_t Admin, uint8_t Id, char * Name);
void themvantay();
void xoavantay();
void xoavantayall();
void vantay();
void suavantay();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t start = 0;
uint8_t data[] = "HELLO";

uint8_t Mode = 0;
uint8_t Admin = 0;
uint8_t Id = 0;
uint8_t Name = 0;
int IDE;
int y;
int i, m; // quet phim i, j va m gia tri tung phan tu cua mang
int value; // gia tri ascii cua ban phim
int l = 1000; // sô lan vong lap for quet phim
char msg[30];
int delay_phim = 100;
int col = 1;
int	row = 0;
bool name_ok = false;
//char ten[30] = "";

int mode_vantay = 0;// bien mode
bool ok = false;


uint8_t buf4[25];
uint8_t rx_data;
char rx_buffer[100];
unsigned int rx_index = 0;

cJSON *str_json;


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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_Base_Start_IT(&htim2);
	// khoi tao LCD
  lcd_init();
	lcd_put_cur(0, 0);
	lcd_send_string("DO AN TOT NGHIEP");
	HAL_Delay(3000);
	lcd_clear();
	// set keypad
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	
	m = 0;
	lcd_put_cur(0, 0);
	char ten[30]="";
	//lcd_send_string(">");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*nutnhan();
		if (mode_vantay == 1 && ok == true){
			themvantay();
		}
		else if (mode_vantay == 2 && ok == true){
			xoavantay();
		}
		else if (mode_vantay == 3 && ok == true){
			xoavantayall();
		}
		else if (mode_vantay == 4 && ok == true){
			vantay();
		}
		else if (mode_vantay == 5 && ok == true){
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string(">");
			while (ok == true)
			{
				//quetphim();
			}
		}*/
		quetphim(ten);
  /* USER CODE END 3 */
}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  htim2.Init.Prescaler = 7200;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
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
  * @brief USART1 Initialization Function
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB10 PB11 
                           PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void quetphim(char * ten)
{
	 /*
		A4-A7 -> output  // moi dau set 1 het
		PB0, PB4, PB5 -> input
	*/
	////////////// QUET VONG 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	/* KEY1 */
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) // chon mode
	{
		value = 49; // so 1
		HAL_Delay(10);
		lcd_put_cur(row, col);
		lcd_send_data(msg[m] = value);
		value--;
		if(value==32)
        value=49;
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0);
		m++;
		i = 0;
		col++;
		HAL_Delay(delay_phim); 
	}
	
	/* KEY2 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
	{
		value = 65; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 68)
          value = 50;
        if(value == 51)
          value = 65;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
	
	/* KEY3 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0) 
	{
		value = 68; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 71)
          value = 51;
        if(value == 52)
          value = 68;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0);
				i = 0; 
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++; // tang cot lcd
	}
	/////////////// QUET VONG 2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			/* KEY4 */
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) 
	{
		value = 71; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 74)
          value = 52;
        if(value == 53)
          value = 71;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
		/* KEY5 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0) 
	{
		value = 74; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 77)
          value = 53;
        if(value == 54)
          value = 74;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
			/* KEY6 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
	{
		value = 77; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 80)
          value = 54;
        if(value == 55)
          value = 77;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
	
	//////////// QUET VONG 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	/* KEY7 */
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) 
	{
		value = 80; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 84)
          value = 55;
        if(value == 56)
          value = 80;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
		/* KEY8 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0) 
	{
		value = 84; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 87)
          value = 56;
        if(value == 57)
          value = 84;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
			/* KEY9 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0) 
	{
		value = 87; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value++;
        if(value == 91)
          value = 57;
        if(value == 58)
          value = 87;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}	
	///////// QUET LAN 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	
	/* KEY  * */
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) 
	{
		ok = false;
		name_ok = true; // nhap ten xong
		lcd_clear();
		lcd_put_cur(0,0);
		lcd_send_string(">>");
		lcd_put_cur(0,2);

		for(int a = 0; a<m; a++)
		{
			lcd_send_data(msg[a]);
			ten[a]+=msg[a];
		}
		/* NHAP TEN OK THI NHAP THEM VAN TAY*/
		HAL_Delay(1000);

		for(int a = 0; a<30; a++)  // clear name buffer
		{
			msg[a] = 0; // clear mess
			m = 0; 
		}
		col = 0;
		row = 0;
		lcd_clear();
		lcd_put_cur(row, col);
		lcd_send_string(">");
		col++;
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0);
	}
		/* KEY0 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
	{
		value = 48; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0)
			{
				lcd_put_cur(row, col);
				lcd_send_data(msg[m] = value);
				value = value - 16;
        if(value < 32)  // laanf hai nhan value = 48 + 16
          value = 48;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
			/* KEY #   delete */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0) 
	{
		if(m != 0)
			m--;
		if (col == 0 && row == 1)
		{
			col = 16;
			row = 0;
		}
		if (row == 0 && col == 1)
		{}
		else if (col >= 1)
			col--;
		lcd_put_cur(row, col);
		lcd_send_string(" ");
		lcd_put_cur(row, col);
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0);
		HAL_Delay(delay_phim);
	}	
	if(col > 15)
	{
		col = 0;
		row = 1;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}

void nutnhan()
{
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) // chon mode
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		mode_vantay+=1;
		if (mode_vantay == 6){
			mode_vantay = 1;
		}
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
	}
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)  // bool OK
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		ok = true;
		lcd_clear(); // clear menu
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0);
	}
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0)
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0);
	}
	
	if (mode_vantay == 1){
		lcd_put_cur(0, 0);
		lcd_send_string("|> THEM VAN TAY   ");
		lcd_put_cur(1, 0);
		lcd_send_string("|  XOA VAN TAY   ");
	}
	else if (mode_vantay == 2){
		lcd_put_cur(0, 0);
		lcd_send_string("|  THEM VAN TAY   ");
		lcd_put_cur(1, 0);
		lcd_send_string("|> XOA VAN TAY   ");
	}
	else if (mode_vantay == 3){
		lcd_put_cur(0, 0);
		lcd_send_string("|  XOA VAN TAY   ");
		lcd_put_cur(1, 0);
		lcd_send_string("|> XOA TOAN BO   ");
	}
	else if (mode_vantay == 4){
		lcd_put_cur(0, 0);
		lcd_send_string("|  XOA TOAN BO   ");
		lcd_put_cur(1, 0);
		lcd_send_string("|>  DIEM DANH    ");
	}
	else if (mode_vantay == 5){
		lcd_put_cur(0, 0);
		lcd_send_string("|   DIEM DANH    ");
		lcd_put_cur(1, 0);
		lcd_send_string("|>  NHAP TEN     ");
	}
}

void suavantay()
{

}
void vantay()
{	
		if(verifyPassword() == 1 )
		{
			IDE = fingerIDSearch();  // ID
			if(IDE == -1)
			{
				//HAL_Delay(1000);
			}
			else if (IDE == 999) // thoat mode diem danh
			{
				ok = false;
			}
			else   //////// doc thu cong // truy xuat sql
			{
				if(IDE == 0 )
				{
					lcd_put_cur(0, 0);
					lcd_send_string("    DIEM DANH    ");
					lcd_put_cur(1, 0);
					lcd_send_string("  FOUND ADMIN     ");
					HAL_Delay(2000);
				}

				if(IDE == 1)
				{
					lcd_put_cur(0, 0);
					lcd_send_string("    DIEM DANH    ");
					lcd_put_cur(1, 0);
					lcd_send_string("  FOUND NGHIA     ");
					HAL_Delay(2000);
				}
				if(IDE==3)
				{
					lcd_put_cur(0, 0);
					lcd_send_string("   DIEM DANH    ");
					lcd_put_cur(1, 0);
					lcd_send_string("  FOUND MINH     ");
					HAL_Delay(2000);
				}
			}
		}
}
void themvantay()
{	
	lcd_clear();
	lcd_put_cur(0, 0);
	lcd_send_string("  XAC NHAN ADMIN  ");	
	if(verifyPassword() == 1 ) // xac nhan admin
	{
		IDE = fingerIDSearch();
		if (IDE == 0) // bang 0: ADMIN
		{
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("  THEM VAN TAY  ");
			HAL_Delay(1000);
			lcd_clear();
			/*DOC BAN PHIM NHAP VAO TEN */
			lcd_put_cur(0, 0);
			lcd_send_string(">");
			name_ok = false;
			char ten[30] = "";
			bool chay_van_tay = false;
			while (name_ok == false)// nhap cho xong ten
			{
				quetphim(ten);
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat nhap ten
				{	
					HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
					name_ok = true;
					chay_van_tay = true;
				}
			}
			if (chay_van_tay == false) // neu khong nhap ten thi khog nhap van tay
			{
				if(verifyPassword() == 1 )
				{	
					int	v = fingerIDSearch(); // tim xem co van tay khong
					if (v == -1)
					{ // neu co van tay roi
						IDE = Flash_Read_Int(ADDRESS_DATA_STORAGE);
						int x = fingerEnroll(IDE);
						if ( x == 99)
						{
							ok = false;
						}
						else if (x == 0)
						{
							lcd_put_cur(1,1);
							lcd_send_string("       LOI      ");
							HAL_Delay(1000);
						}
						else if(x == 1 )// XÁC NHAN NHAP DUNG VAN TAY HAI LAN
						{
							//lcd_clear();
							lcd_put_cur(1,1);
							lcd_send_string("THEM OK ID: ");
							lcd_send_data(IDE/10 + 0x30);
							lcd_send_data(IDE%10 + 0x30);
							/*send name, IDE, Mode den serial*/
							send_data_uart1(mode_vantay, Admin, IDE, ten);
							HAL_Delay(1000);
							IDE++;
							Flash_Erase(ADDRESS_DATA_STORAGE);
							Flash_Write_Int(ADDRESS_DATA_STORAGE,IDE);
		
						}
						ok = false;
					}
					else if (v == 999){
						ok = false;
					}
					else {
						lcd_put_cur(1,1);
						lcd_send_string("  DA TON TAI ");
						HAL_Delay(1000);
					}
				}
			}
					
		lcd_clear();
		}
		else if (IDE == 999)
		{
			ok = false;
		}
		else 
		{
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("      SAI ADMIN     ");	
			HAL_Delay(2000);
		}
	}
}
void xoavantay()
{
	lcd_clear();
	lcd_put_cur(0, 0);
	lcd_send_string("  XAC NHAN ADMIN  ");	
	if(verifyPassword() == 1 ) // xac nhan admin
	{
		IDE = fingerIDSearch();
		if (IDE == 0) // bang 0: ADMIN
		{
			HAL_Delay(1000);
			if(verifyPassword() == 1 )
			{	
				lcd_clear();
				lcd_put_cur(0, 0);
				lcd_send_string("   XOA VAN TAY  ");
				lcd_put_cur(1,0);
				lcd_send_string("MOI DAT TAY VAO");
				HAL_Delay(1000);
				y = fingerIDSearch();
				if (y == 999)
				{
					// thoat khoi xoa van tay
				}
				else if (y == 0)
				{
					lcd_put_cur(0,0);
					lcd_send_string("   KHONG THE   ");
					lcd_put_cur(1,0);
					lcd_send_string("XOA VAN TAY ADMIN");
					HAL_Delay(1000);
				}
				else if (y != -1) // co van tay thi xoa
				{
					deleteModel(y);
					lcd_put_cur(1,0);
					lcd_send_string("  DA XOA  ");
					lcd_put_cur(1,8);
					lcd_send_string("  ID: ");
					lcd_send_data(y/10 + 0x30);
					lcd_send_data(y%10 + 0x30);
					HAL_Delay(1000);
				}
				else { // khong co van tay
					lcd_put_cur(0,0);
					lcd_send_string("  VAN TAY SAI  ");
					lcd_put_cur(1,0);
					lcd_send_string("XIN HAY THU LAI");
					HAL_Delay(1000);
				}
				lcd_clear ();
				ok = false;
			}
		}
		else if (IDE == 999)
		{
			ok = false;
		}
		else 
		{
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("    SAI ADMIN   ");	
			HAL_Delay(2000);
		}
	}
}

void xoavantayall()
{
	lcd_clear();
	lcd_put_cur(0, 0);
	lcd_send_string("  XAC NHAN ADMIN  ");	
	if(verifyPassword() == 1 ) // xac nhan admin
	{
		IDE = fingerIDSearch();
		if (IDE == 0) // bang 0: ADMIN
		{
			if(verifyPassword() == 1 )
			{
				emptyDatabase();
				lcd_clear();
				lcd_put_cur(1,1);
				lcd_send_string("     DA XOA     ");
				HAL_Delay(1000);
				lcd_clear ();
				ok = false;
				IDE=0;
				Flash_Erase(ADDRESS_DATA_STORAGE);
				Flash_Write_Int(ADDRESS_DATA_STORAGE,0);
			}
		}
	  else 
		{
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("    SAI ADMIN   ");	
			HAL_Delay(2000);
		}
	}
}
void send_data_uart(uint8_t Mode, uint8_t Admin, uint8_t Id, uint8_t Name)
{
	char JSON[100] = "";
	char Str_Mode[10] = ""; 
	char Str_Admin[10] = "";
	char Str_Id[10] = "";
	char Str_Name[10] = "";

	sprintf(Str_Mode, "%d", Mode); 
	sprintf(Str_Admin, "%d", Admin);
	sprintf(Str_Id, "%d", Id);
	sprintf(Str_Name, "%d", Name);
	// {\"TB1\":"1"}
	strcat(JSON, "{\"Mode\":\"");
	strcat(JSON, Str_Mode); strcat(JSON, "\",");
	strcat(JSON, "\"Admin\":\"");
	strcat(JSON, Str_Admin); strcat(JSON, "\",");

	strcat(JSON, "\"Id\":\"");
	strcat(JSON, Str_Id); strcat(JSON, "\",");
	strcat(JSON, "\"Name\":\"");
	strcat(JSON, Str_Name); strcat(JSON, "\"}\n");
	HAL_UART_Transmit(&huart2, JSON, sizeof(JSON), 10); // gui 500ms 1 lan // ,10 timeout 
}
void send_data_uart1(uint8_t Mode, uint8_t Admin, uint8_t Id, char * Name)
{
	char JSON[80] = "";
	char Str_Mode[2] = ""; 
	char Str_Admin[2] = "";
	char Str_Id[2] = "";
	//char Str_Name[30] = "";

	sprintf(Str_Mode, "%d", Mode); 
	sprintf(Str_Admin, "%d", Admin);
	sprintf(Str_Id, "%d", Id);
	// 
	strcat(JSON, "{\"Mode\":\"");
	strcat(JSON, Str_Mode); strcat(JSON, "\",");
	strcat(JSON, "\"Admin\":\"");
	strcat(JSON, Str_Admin); strcat(JSON, "\",");

	strcat(JSON, "\"Id\":\"");
	strcat(JSON, Str_Id); strcat(JSON, "\",");
	strcat(JSON, "\"Name\":\"");
	strcat(JSON, Name); strcat(JSON, "\"}\n");
	HAL_UART_Transmit(&huart2, JSON, sizeof(JSON), 10); // gui 500ms 1 lan // ,10 timeout 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // call back interupt timer
{
	if(htim->Instance == htim2.Instance){
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//send_data_uart(mode_vantay, Admin, IDE, Name);
	}
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
