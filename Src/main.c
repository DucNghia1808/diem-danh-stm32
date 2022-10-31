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
//#include "i2c-lcd.h"
#include "cJSON.h"
//#include "usart.h"
#include "R305.h"
#include "flash.h"
#include "ringbuf.h"
#include "LCD_I2C.h"
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

void quetphim(char *ten);
void quetphimcontrol();


void send_data_uart(uint8_t Mode, uint8_t Admin, uint8_t Id, uint8_t Name);
void send_data_uart1(uint8_t Mode, uint8_t Admin, uint8_t Id, char * Name);
void themvantay();
void xoavantay();
void xoavantayall();
void vantay();
void suavantay();
void xoavantaytuPC(int id);
void xulyJSON(char *dataJson);
void Received(void);

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
int l = 1000; // s� lan vong lap for quet phim
char msg[30];
int delay_phim = 100;
int col = 1;
int	row = 1;
bool name_ok = false;


//char ten[30] = "";

int mode_vantay = 0;// bien mode
bool ok = false;
bool phai_them_vantay_ad = false;
bool chuathemvantayduoc = false;
char tentuPC[50];

uint8_t idxoa;
uint8_t buf4[25];

uint8_t rx_data;
char rx_buffer[50];
unsigned int rx_index = 0;

cJSON *str_json;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // call back uart interrupt
{
	if (huart -> Instance == USART2){
		Received();// nhan + xu ly data
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);  // nhan data uart interrupt/ 1: data size
	}
}

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
	HAL_UART_Receive_IT(&huart2, &rx_data, 1); // nhan data uart
	// khoi tao LCD
	
	LCD_Init();
	LCD_Clear();
	LCD_setCursor(0, 0);
	LCD_sendString("");
	LCD_Clear();
	LCD_setCursor(0, 0);
	LCD_sendString("  DO AN TOT NGHIEP  ");
	HAL_Delay(1000);
	// set keypad
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	
	m = 0;

	char ten[30]="";

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	LCD_CMD(0x0C);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		quetphimcontrol();
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
			LCD_setCursor(0, 0);
			LCD_sendString("DIEM DANH NHAN VIEN");
			vantay();
		}
		else if (mode_vantay == 5 && ok == true){
			LCD_Clear();
			LCD_setCursor(0, 0);
			LCD_sendString(">");
			while (ok == true)
			{
				//quetphim();
			}
		}
		else {
			if (mode_vantay == 1){
				LCD_setCursor(1, 0);
				LCD_sendString("|>  THEM VAN TAY   ");
				LCD_setCursor(2, 0);
				LCD_sendString("|    XOA VAN TAY   ");
				LCD_setCursor(3, 0);
				LCD_sendString("|    XOA TOAN BO   ");
			}
			else if (mode_vantay == 2){
				LCD_setCursor(1, 0);
				LCD_sendString("|   THEM VAN TAY   ");
				LCD_setCursor(2, 0);
				LCD_sendString("|>   XOA VAN TAY   ");
				LCD_setCursor(3, 0);
				LCD_sendString("|    XOA TOAN BO   ");
			}
			else if (mode_vantay == 3){
				LCD_setCursor(1, 0);
				LCD_sendString("|   THEM VAN TAY   ");
				LCD_setCursor(2, 0);
				LCD_sendString("|    XOA VAN TAY   ");
				LCD_setCursor(3, 0);
				LCD_sendString("|>   XOA TOAN BO   ");
			}
			else if (mode_vantay == 4){
				LCD_setCursor(1, 0);
				LCD_sendString("|    XOA VAN TAY   ");
				LCD_setCursor(2, 0);
				LCD_sendString("|    XOA TOAN BO   ");
				LCD_setCursor(3, 0);
				LCD_sendString("|>    DIEM DANH    ");
			}
			else if (mode_vantay == 5){	
				LCD_setCursor(1, 0);
				LCD_sendString("|    XOA TOAN BO   ");
				LCD_setCursor(2, 0);
				LCD_sendString("|     DIEM DANH    ");
				LCD_setCursor(3, 0);
				LCD_sendString("|>    NHAP TEN     ");
			}
			else 
			{
				LCD_setCursor(0, 0);
				LCD_sendString("  DO AN TOT NGHIEP  ");
			}
		}
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
  htim2.Init.Period = 4999;
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
	 /* A4-A7 -> output  // moi dau set 1 het
		PB0, PB1, PB10 PB11 -> input */
	////////////// QUET VONG 1
	LCD_CMD(0x0D);
	if (m<20){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	/* KEY1 */

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) // chon mode
	{
		value = 49; // so 1
		HAL_Delay(10);
		LCD_setCursor(2,0);
		msg[m] = value;
		LCD_sendString(msg);
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
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
	{
		value = 65; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);
				value++;
        if(value == 68)
          value = 50;
        if(value == 51)
          value = 65;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
	
	/* KEY3 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) 
	{
		value = 68; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);
				value++;
        if(value == 71)
          value = 51;
        if(value == 52)
          value = 68;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
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
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);
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
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0) 
	{
		value = 74; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);
				value++;
        if(value == 77)
          value = 53;
        if(value == 54)
          value = 74;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
			/* KEY6 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
	{
		value = 77; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);
				value++;
        if(value == 80)
          value = 54;
        if(value == 55)
          value = 77;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
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
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);	
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
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0) 
	{
		value = 84; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);		
				value++;
        if(value == 87)
          value = 56;
        if(value == 57)
          value = 84;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
			/* KEY9 */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) 
	{
		value = 87; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);			
				value++;
        if(value == 91)
          value = 57;
        if(value == 58)
          value = 87;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}	
	
	//////// QUET LAN 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	
	/* KEY  * */
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) 
	{
		ok = false;
		name_ok = true; // nhap ten xong
		col = 0;
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0);
		LCD_CMD(0x0C);
		LCD_Clear();
		LCD_setCursor(0, 0);
		LCD_sendString("   THEM VAN TAY   ");
		LCD_setCursor(2, 0);
	}
		/* KEY0 */
	
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
	{
		value = 48; // so 1
		HAL_Delay(10);
		for (i=0; i<l; i++)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0)
			{
				LCD_setCursor(2,0);
				msg[m] = value;
				LCD_sendString(msg);		
				value = value - 16;
        if(value < 32)  // laanf hai nhan value = 48 + 16
          value = 48;
				while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 0);
				i = 0;
				HAL_Delay(delay_phim);
			}
		}
		m++;
		col++;
	}
			/* KEY #   delete */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) 
	{
		if(m != 0){
			m--;
			col--;
		}
		msg[m] = 0;
		LCD_Write(msg[m]);
		LCD_Clear();
		LCD_setCursor(1, 0);
		LCD_sendString("   NHAP VAO TEN  ");
		LCD_setCursor(0, 0);
		LCD_sendString("   THEM VAN TAY   ");
		LCD_setCursor(2, 0);
		for(int t=0; t<m; t++){
				LCD_Write(msg[t]);
		}
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
		HAL_Delay(delay_phim);
	}	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}
	else{//m >20
			//////// QUET LAN 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	
	/* KEY * */
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0) 
	{
		ok = false;
		name_ok = true; // nhap ten xong
		col = 0;
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0);
		LCD_CMD(0x0C);
		LCD_Clear();
		LCD_setCursor(0, 0);
		LCD_sendString("   THEM VAN TAY   ");
		LCD_setCursor(2, 0);
	}
			/* KEY #   delete */
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0) 
	{
		if(m != 0){
			m--;
			col--;
		}
		msg[m] = 0;
		LCD_Write(msg[m]);
		LCD_Clear();
		LCD_setCursor(1, 0);
		LCD_sendString("   NHAP VAO TEN  ");
		LCD_setCursor(0, 0);
		LCD_sendString("   THEM VAN TAY   ");
		LCD_setCursor(2, 0);
		for(int t=0; t<m; t++){
				LCD_Write(msg[t]);
		}
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
		HAL_Delay(delay_phim);
	}	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	}
}


void quetphimcontrol()
{
	/* QUET VONG 1*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // chon mode +
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		mode_vantay+=1;
		if (mode_vantay == 6){
			mode_vantay = 1;
		}
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0);
	}
	
	/* QUET VONG 2*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // chon mode -
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		if (mode_vantay == 1){
			mode_vantay = 6;
		}
		mode_vantay-=1;

		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0);
	}
	/* QUET VONG 3*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // chon OK
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		ok = true;
		LCD_Clear(); // clear menu
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0);
	}
	/* QUET VONG 4*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // chon THOAT
	{
		HAL_Delay(20);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		LCD_CMD(0x0C);
		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0);
	}
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

}

void suavantay()
{

}

///////////// JSON
void xulyJSON(char *dataJson)
{
	str_json = cJSON_Parse(dataJson);
	if(!str_json) // khong phai chuoi JSON
	{

	}
	else  // la chuoi JSON
	{
		if (cJSON_GetObjectItem(str_json, "Xoa")) /// xoa ID
		{
			idxoa = atoi(cJSON_GetObjectItem(str_json, "Xoa")->valuestring); // atoi convert string to interger		
			xoavantaytuPC(idxoa); // xoa id van tay
		}
		if (cJSON_GetObjectItem(str_json, "OK")) /// xoa ID
		{
			chuathemvantayduoc = true;
		}
		if (cJSON_GetObjectItem(str_json, "NAME")) /// xoa ID
		{	
			LCD_setCursor(2,0);
			LCD_sendString("  XIN CAM ON");
			LCD_setCursor(3,8);
			LCD_sendString(cJSON_GetObjectItem(str_json, "NAME")->valuestring);
		}
	}
	cJSON_Delete(str_json); // x�a m?ng json
}
void Received(void)
{
	if (rx_data != '\n')
	{
			rx_buffer[rx_index++] = rx_data;
	}
	else 
	{
		xulyJSON(rx_buffer);
		//printf("Data: %s\n",rx_buffer );
		rx_index = 0;	
		for (int i = 0; i < 50; i++)
		{
			rx_buffer[i] = 0;
		}
	}
}

void xoavantaytuPC(int id)
{		if(id != 0){
			deleteModel(id);
			LCD_setCursor(3,0);
			LCD_sendString("  DA XOA");
			LCD_setCursor(3,8);
			LCD_sendString(" ID: ");
			LCD_Write(id/10 + 0x30);
			LCD_Write(id%10 + 0x30);
			send_data_uart1(2,3,id, "");
		}
}
void vantay()
{		
		if(verifyPassword() == 1 )
		{
			IDE = fingerIDSearch();  // ID
			if(IDE == -1)
			{
				//HAL_Delay(1000);
					LCD_setCursor(1, 0);
					LCD_sendString("    VAN TAY SAI   ");
					LCD_setCursor(2, 0);
					LCD_sendString("  XIN HAY THU LAI  ");
					HAL_Delay(2000);
					LCD_Clear();
			}
			else if (IDE == 999) // thoat mode diem danh
			{
				ok = false;
			}
			else   //////// doc thu cong // truy xuat sql
			{
				send_data_uart1(3,1 , IDE, ""); // gui ID truy
				HAL_Delay(2000);
				LCD_Clear();
			}
		}
}
void themvantay()
{	
	chuathemvantayduoc = false;
	if(phai_them_vantay_ad == false){
		LCD_Clear();
		LCD_setCursor(0, 0);
		LCD_sendString("   XAC NHAN ADMIN  ");	
		if(verifyPassword() == 1 ) // xac nhan admin
		{
			IDE = fingerIDSearch();
			if (IDE == 0) // bang 0: ADMIN
			{
				LCD_Clear();
				LCD_setCursor(0, 0);
				LCD_sendString("    THEM VAN TAY   ");
				/*DOC BAN PHIM NHAP VAO TEN */
				LCD_setCursor(1, 0);
				LCD_sendString("   NHAP VAO TEN  ");
				LCD_setCursor(2, 0);
				LCD_sendString(">");
				name_ok = false;
				char ten[30] = "";
				bool chay_van_tay = false;
				while (name_ok == false)// nhap cho xong ten
				{
					quetphim(ten);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
					if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat nhap ten
					{	
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
						name_ok = true; // add ten van tay
						chay_van_tay = true;  // chay add van tay cam bien
						ok = false; // thoat
					}
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
				}
				if (chay_van_tay == false) // neu khong nhap ten thi khog nhap van tay
				{	
					LCD_setCursor(1, 0);
					LCD_sendString("  NHAP VAO VAN TAY  ");
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
								LCD_setCursor(3,0);
								LCD_sendString("     LOI KHI THEM   ");
								HAL_Delay(1000);
							}
							else if(x == 1 )// X�C NHAN NHAP DUNG VAN TAY HAI LAN
							{

								/*send name, IDE, Mode den serial*/
										send_data_uart1(1, Admin, IDE, msg);
																	//LCD_Clear();
										LCD_setCursor(3,1);
										LCD_sendString("  THEM OK ID: ");
										LCD_Write(IDE/10 + 0x30);
										LCD_Write(IDE%10 + 0x30);
										for(int a = 0; a<20; a++)  // clear name buffer
										{
											msg[a] = 0; // clear mess
											m = 0; 
										}
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
							LCD_setCursor(3,0);
							LCD_sendString(" DA TON TAI VAN TAY");
							HAL_Delay(1000);
						}
					}
				}
						
			LCD_Clear();
			}
			else if (IDE == 999)
			{
				ok = false;
			}
			else 
			{
				LCD_Clear();
				LCD_setCursor(3, 0);
				LCD_sendString("      SAI ADMIN    ");	
				HAL_Delay(2000);
			}
		}
	}
	else { /// them vay tay khong xac nhan admin
		LCD_Clear();
		LCD_setCursor(0, 0);
		LCD_sendString(" THEM VAN TAY ADMIN   ");
		/*DOC BAN PHIM NHAP VAO TEN */
		LCD_setCursor(1, 0);
		LCD_sendString("   NHAP VAO TEN  ");
		LCD_setCursor(2, 0);
		LCD_sendString(">");
		name_ok = false;
		char ten[30] = "";
		bool chay_van_tay = false;
		while (name_ok == false)// nhap cho xong ten
		{
			quetphim(ten);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat nhap ten
			{	
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
				name_ok = true; // add ten van tay
				chay_van_tay = true;  // chay add van tay cam bien
				ok = false; // thoat
			}
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		}
		if (chay_van_tay == false) // neu khong nhap ten thi khog nhap van tay
		{	
			LCD_setCursor(1, 0);
			LCD_sendString("  NHAP VAO VAN TAY  ");
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
						LCD_setCursor(3,0);
						LCD_sendString("     LOI KHI THEM   ");
						HAL_Delay(1000);
					}
					else if(x == 1 )// X�C NHAN NHAP DUNG VAN TAY HAI LAN
					{ 
						send_data_uart1(1, Admin, IDE, msg);
						LCD_setCursor(2,1);
						LCD_sendString("   THEM OK ADMIN  ");						
						LCD_setCursor(3,1);
						LCD_sendString("  THEM OK ID: ");
						LCD_Write(IDE/10 + 0x30);
						LCD_Write(IDE%10 + 0x30);
						/*send name, IDE, Mode den serial*/

						for(int a = 0; a<20; a++)  // clear name buffer
						{
							msg[a] = 0; // clear mess
							m = 0; 
						}
						HAL_Delay(1000);
						IDE++;
						Flash_Erase(ADDRESS_DATA_STORAGE);
						Flash_Write_Int(ADDRESS_DATA_STORAGE,IDE);
						phai_them_vantay_ad = false;
					}
					ok = false;
				}
				else if (v == 999){
					ok = false;
				}
				else {
					LCD_setCursor(3,0);
					LCD_sendString(" DA TON TAI VAN TAY");
					HAL_Delay(1000);
				}
			}
		}		
	LCD_Clear();
	}
}
void xoavantay()
{
	LCD_Clear();
	LCD_setCursor(0, 0);
	LCD_sendString("   XAC NHAN ADMIN  ");	
	if(verifyPassword() == 1 ) // xac nhan admin
	{
		IDE = fingerIDSearch();
		if (IDE == 0) // bang 0: ADMIN
		{
			HAL_Delay(1000);
			if(verifyPassword() == 1 )
			{	
				LCD_Clear();
				LCD_setCursor(0, 0);
				LCD_sendString("     XOA VAN TAY   ");
				LCD_setCursor(1,0);
				LCD_sendString("   MOI DAT TAY VAO ");
				HAL_Delay(1000);
				y = fingerIDSearch();
				if (y == 999)
				{
					// thoat khoi xoa van tay
				}
				else if (y == 0)
				{
					LCD_setCursor(2,0);
					LCD_sendString("      KHONG THE    ");
					LCD_setCursor(3,0);
					LCD_sendString("  XOA VAN TAY ADMIN");
					HAL_Delay(1000);
				}
				else if (y != -1) // co van tay thi xoa
				{
					deleteModel(y);
					LCD_setCursor(2,0);
					LCD_sendString("  DA XOA");
					LCD_setCursor(2,8);
					LCD_sendString(" ID: ");
					LCD_Write(y/10 + 0x30);
					LCD_Write(y%10 + 0x30);
					send_data_uart1(2, Admin, y,"");
					HAL_Delay(1000);
				}
				else { // khong co van tay
					LCD_setCursor(2,0);
					LCD_sendString("     VAN TAY SAI   ");
					LCD_setCursor(3,0);
					LCD_sendString("   XIN HAY THU LAI ");
					HAL_Delay(1000);
				}
				LCD_Clear ();
				ok = false;
			}
		}
		else if (IDE == 999)
		{
			ok = false;
		}
		else 
		{
			LCD_Clear();
			LCD_setCursor(3 , 0);
			LCD_sendString("      SAI ADMIN    ");	
			HAL_Delay(2000);
		}
	}
}

void xoavantayall()
{
	if (phai_them_vantay_ad == false)
	{ // 
		LCD_Clear();
		LCD_setCursor(0, 0);
		LCD_sendString("   XAC NHAN ADMIN  ");	
		if(verifyPassword() == 1 ) // xac nhan admin
		{
			IDE = fingerIDSearch();
			if (IDE != 0) // bang 0: ADMIN
			{
				if(verifyPassword() == 1 )
				{
					emptyDatabase();
					LCD_Clear();
					LCD_setCursor(2,1);
					LCD_sendString("   DA XOA ALL   ");
					HAL_Delay(1000);
					LCD_Clear ();
					ok = false;
					phai_them_vantay_ad = true;
					IDE=0;
					Flash_Erase(ADDRESS_DATA_STORAGE);
					Flash_Write_Int(ADDRESS_DATA_STORAGE,0);
					// sau khi xoa toan bo them van tay admin luon
					// GUI TIN HIEU XOA ALL CHO
				}
			}
			else 
			{
				LCD_Clear();
				LCD_setCursor(3 , 0);
				LCD_sendString("      SAI ADMIN    ");	
				HAL_Delay(2000);
				ok = false;
			}
		}
	}
	else 
	{
			LCD_setCursor(3 , 0);
			LCD_sendString("  CHUA CO VAN TAY  ");
			HAL_Delay(1000);
			ok = false;
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
	strcat(JSON, Str_Name); strcat(JSON, "\"}\n\n");
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
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
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
