#ifndef LCD_I2C
#define LCD_I2C

#include "stm32f1xx_hal.h"

//#define ADDRESS_LCD 0x4E

void LCD_CMD(uint8_t cmd);
void LCD_Write(uint8_t data);
void LCD_Init(void);
void LCD_setCursor(uint8_t row, uint8_t col);
void LCD_sendString(char *str);
void LCD_Clear(void);
void LCD_setCursor_NGU(uint8_t row, uint8_t col);	
#endif
