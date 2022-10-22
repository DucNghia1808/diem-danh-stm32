#include "stm32f1xx_hal.h"
#include "LCD_I2C.h"

#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODE      0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSOR				 0x10
#define LCD_FUNCTIONSET    0X20

#define LCD_DISPLAY_D      0x04
#define LCD_DISPLAY_C      0x02
#define LCD_DISPLAY_B      0x01

#define LCD_FUNCTION_F     0x04
#define LCD_FUNCTION_N     0x08
#define LCD_FUNCTION_DL    0x10

#define LCD_LIGHT (1 << 3)
#define LCD_EN    (1 << 2)
#define LCD_RS    (1 << 0)

#define ADDRESS_LCD 0x4E

extern I2C_HandleTypeDef hi2c1;

void LCD_CMD(uint8_t cmd)
{
	uint8_t cmd_4_7 = (0xF0 & cmd);
	uint8_t cmd_0_3 = (0xF0 & (cmd << 4));
	
	uint8_t sendCmd[4] = 
	{
		cmd_4_7 | LCD_EN | LCD_LIGHT,
		cmd_4_7 | LCD_LIGHT,
		cmd_0_3 | LCD_EN | LCD_LIGHT,
		cmd_0_3 | LCD_LIGHT,
	};
	
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS_LCD, sendCmd, sizeof(sendCmd), 1000);
}

void LCD_Write(uint8_t data)
{
	uint8_t data_4_7 = (0xF0 & data);
	uint8_t data_0_3 = (0xF0 & (data << 4));
	
	uint8_t sendData[4] = 
	{
		data_4_7 | LCD_EN | LCD_LIGHT| LCD_RS,
		data_4_7 | LCD_LIGHT| LCD_RS,
		data_0_3 | LCD_EN | LCD_LIGHT | LCD_RS,
		data_0_3 | LCD_LIGHT | LCD_RS
	};
	
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS_LCD, sendData, sizeof(sendData), 1000);
}

void LCD_Init(void)
{
	HAL_Delay(40); // Delay 40ms after power-up
	
	LCD_CMD(0x30); 
	HAL_Delay(15); // Delay 5ms
	
	LCD_CMD(0x30); 
	HAL_Delay(15);
	
	LCD_CMD(0x30); 
	HAL_Delay(15);
	
	LCD_CMD(0x20);
	HAL_Delay(10);
	
	
	//////////////////
	//LCD_CMD(LCD_FUNCTIONSET | LCD_FUNCTION_N);
	LCD_CMD(0x28);
	HAL_Delay(10);
	
	//LCD_CMD(LCD_DISPLAYCONTROL);
	LCD_CMD(0x08);
	HAL_Delay(10);
	
	//LCD_CMD(LCD_CLEARDISPLAY);
	LCD_CMD(0x01);
	HAL_Delay(10);
	
	LCD_CMD(0x06); // tu dong di chuyen con tro den vi tri tiep theo lcd
	HAL_Delay(10);
	
	LCD_CMD(LCD_DISPLAYCONTROL | LCD_DISPLAY_D);
	//LCD_CMD(0x0C);
	HAL_Delay(10);

}
void LCD_setCursor_NGU(uint8_t row, uint8_t col)
{
	uint8_t cmd; 
	if(row == 1)
	{
		switch (col){
			case 1:
					cmd = 74;
					break;
			case 2:
					cmd = 75;
					break;
			case 3:
					cmd = 76;
					break;
			case 4:
					cmd = 77;
					break;
			case 5:
					cmd = 78;
					break;
			case 6:
					cmd = 79;
					break;
			case 7:
					cmd = 80;
					break;
			case 8:
					cmd = 81;
					break;
			case 9:
					cmd = 82;
					break;
			case 10:
					cmd = 83;
					break;
		}
					LCD_CMD(cmd | 0xc0);
	}
	else if (row == 2)
	{
		switch (col){
			case 1:
					cmd = 20;
					break;
			case 2:
					cmd = 21;
					break;
			case 3:
					cmd = 22;
					break;
			case 4:
					cmd = 23;
					break;
			case 5:
					cmd = 24;
					break;
			case 6:
					cmd = 25;
					break;
			case 7:
					cmd = 26;
					break;
			case 8:
					cmd = 27;
					break;
			case 9:
					cmd = 28;
					break;
			case 10:
					cmd = 29;
					break;
			case 11:
					cmd = 30;
					break;
			case 12:
					cmd = 31;
					break;
			case 13:
					cmd = 32;
					break;
			case 14:
					cmd = 33;
					break;
			case 15:
					cmd = 34;
					break;
			case 16:
					cmd = 35;
					break;
			case 17:
					cmd = 36;
					break;
			case 18:
					cmd = 37;
					break;
			case 19:
					cmd = 38;
					break;
			case 20:
					cmd = 39;
					break;
		}
		LCD_CMD(cmd |0x94);
	}

}
void LCD_setCursor(uint8_t row, uint8_t col)
{
  uint8_t maskData;
  maskData = (col)&0x0F;
  if(row==0)
  {
    maskData |= (0x80);
    LCD_CMD(maskData);
  }
  else 
		if(row==1)
			{
				maskData |= (0xc0);
				LCD_CMD(maskData);
			}
			else 
				if(row==2)
				{
					maskData |= (0x94);
					LCD_CMD(maskData);
				}
				else 
				{
					maskData |= (0xD4);
					LCD_CMD(maskData);
				}
				
		/*switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
				case 2:
						col |= 0x94;
						break;
				case 3:
						col |= 0xD4;
						break;
    }
		LCD_CMD(col);*/
}

void LCD_sendString(char *str)
{
	while(*str)
	{
		LCD_Write(*str ++);
	}
}

void LCD_Clear(void)
{
	LCD_CMD(LCD_CLEARDISPLAY);
	HAL_Delay(3);
}
