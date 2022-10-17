#include "R305.h"
#include <stdio.h>
#include "i2c-lcd.h"
#include "main.h"

uint16_t fingerID, confidence;
uint8_t recvPacket[20];
uint32_t thePassword = 0;
uint32_t theAddress = 0xFFFFFFFF;


UART_HandleTypeDef huart1;
/* FINGERPRINT function */
void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}
void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet)
{
	//MX_USART1_UART_Init();
	uint16_t sum;
	uint8_t i;

	uint8_t startcodeH = FINGERPRINT_STARTCODE >> 8;
	uint8_t startcodeL = FINGERPRINT_STARTCODE&0xff;
	uint8_t addr1 = addr >> 24;
	uint8_t addr2 = addr >> 16;
	uint8_t addr3 = addr >> 8;
	uint8_t addr4 = addr&0xff;
	uint8_t lenH = len>>8;
	uint8_t lenL = len&0xff;
	HAL_UART_Transmit(&huart1, &startcodeH, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &startcodeL, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &addr1, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &addr2, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &addr3, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &addr4, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &packettype, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &lenH, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&huart1, &lenL, 1, DEFAULTTIMEOUT);

	sum = (len>>8) + (len&0xff) + packettype;
	uint8_t packet_int8;
	for (i=0; i< len-2; i++)
	{
		packet_int8 = packet[i];
		HAL_UART_Transmit(&huart1, &packet_int8, 1, DEFAULTTIMEOUT);
		sum += packet[i];
	}
	
	uint8_t sumH = sum>>8;
	uint8_t sumL = sum&0xff;
  HAL_UART_Transmit(&huart1, &sumH, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&huart1, &sumL, 1, DEFAULTTIMEOUT);
}
//////// ham thoat khoi mode

uint8_t getReply(uint8_t packet[])
{
  uint8_t reply[20], idx, i, packettype;
	uint16_t len;
	idx = 0;

  while (1)
  {
		//cho RX san sang
    while (huart1.RxState != HAL_UART_STATE_READY) {}
    //nhan packet tu cam bien van tay
    HAL_UART_Receive(&huart1, &reply[idx], 1, DEFAULTTIMEOUT);
    if ((idx == 0) && (reply[0] != (FINGERPRINT_STARTCODE >> 8)))
      continue;
    idx++;
    
    // kiem tra packet!
    if (idx >= 9) {
      if ((reply[0] != (FINGERPRINT_STARTCODE >> 8)) ||
          (reply[1] != (FINGERPRINT_STARTCODE & 0xFF)))
          return FINGERPRINT_BADPACKET;
			
      packettype = reply[6];
      len = reply[7];
			
      len <<= 8;
      len |= reply[8];
      len -= 2;
			
      if (idx <= (len+10)) continue;
      packet[0] = packettype;      
      for (i=0; i<len; i++) {
        packet[1+i] = reply[9+i];
      }
      return len;
    }
  }
}

uint8_t verifyPassword(void)
{
	uint8_t len;
	
	uint8_t packet[] = {FINGERPRINT_VERIFYPASSWORD, 
                      (thePassword >> 24), (thePassword >> 16),
                      (thePassword >> 8), thePassword};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 7, packet);
  len = getReply(recvPacket);
  
  if ((len == 1) && (recvPacket[0] == FINGERPRINT_ACKPACKET) && (recvPacket[1] == FINGERPRINT_OK))
    return 1;

  return 0;
}

int8_t getImage(void)
{
	uint8_t len;
	uint8_t packet[] = {FINGERPRINT_GETIMAGE};
	writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 3, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t image2Tz(uint8_t slot)
{
	uint8_t len;
	uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, slot};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t createModel(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_REGMODEL};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t storeModel(uint16_t id)
{
	uint8_t len;
	uint8_t packet[] = {FINGERPRINT_STORE, 0x02, 0, 0};
	packet[2] = id >> 8;
	packet[3] = id & 0xFF;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t deleteModel(uint16_t id)
{
		uint8_t len;
    uint8_t packet[] = {FINGERPRINT_DELETE, id >> 8, id & 0xFF, 0x00, 0x01};
		packet[1] = id >> 8;
		packet[2] = id & 0xFF;
		
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    len = getReply(recvPacket);
        
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
		return -1;
    return recvPacket[1];
}

int8_t emptyDatabase(void)
{
	uint8_t len;
	uint8_t packet[] = {FINGERPRINT_EMPTY};
	
	writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
	len = getReply(recvPacket);

	if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
	return -1;
	return recvPacket[1];
}

int8_t fingerFastSearch(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_HISPEEDSEARCH, 0x01, 0x00, 0x00, 0x00, 0xA3};
	
	fingerID = 0xFFFF;
  confidence = 0xFFFF;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  fingerID = recvPacket[2];
  fingerID <<= 8;
  fingerID |= recvPacket[3];
  
  confidence = recvPacket[4];
  confidence <<= 8;
  confidence |= recvPacket[5];
  
  return recvPacket[1];
}


uint8_t fingerEnroll(uint8_t id)
{
	int8_t p = -1;
	while (p != FINGERPRINT_OK)
	{
    p = getImage();
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat khoi seach van tay
		{	
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			return 99;
		}
	}
	
	p = image2Tz(1);
	if(p != FINGERPRINT_OK) // luu vao buffer loi
	{ 
		//lcd_clear();
		lcd_put_cur(1,0);
		lcd_send_string("    LOI TAO KI TU     ");
		return 0;
	}
	lcd_put_cur(1,0);
	lcd_send_string(" DANG CAPTURED   ");
	HAL_Delay(1000);
	p = -1;
	while (p != FINGERPRINT_NOFINGER)
	{
   		p = getImage();
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat khoi seach van tay
			{	
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
				return 99;
			}
  	}
	lcd_put_cur(1,0);
	lcd_send_string("    LUU LAN 1    ");

	p = -1;
	while (p != FINGERPRINT_OK) {
    p = getImage();
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat khoi seach van tay
			{	
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
				return 99;
			}
	}
	
	p = image2Tz(2);
	if(p != FINGERPRINT_OK)
	{
		lcd_put_cur(1,0);
		lcd_send_string("    LOI   ");
		return 0;
	}
	
	p = createModel();
	if(p != FINGERPRINT_OK)
	{
		lcd_put_cur(1,0);
		lcd_send_string("  LOI KHI TAO MAU ");
		return 0;
	}
	
	p = storeModel(id);
	if (p == FINGERPRINT_OK)
	{
		//LCD_sendString("THEM THANH CONG");
		return 1;
	}
	else
	{
		//lcd_clear();
		lcd_put_cur(1,0);
		lcd_send_string("    FAILED     ");
		return 0;
	}
}

int fingerIDSearch(void)
{
	int8_t p = -1;
	//lcd_clear();
	//lcd_put_cur(0,0);
	//lcd_send_string("MOI DAT TAY VAO");
	
	while(p != FINGERPRINT_OK)
	{
		p = getImage();
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0) // thoat khoi seach van tay
		{	
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			return 999;
		}
	}

  p = image2Tz(1);
  if (p != FINGERPRINT_OK)
	{
		lcd_clear();
		lcd_put_cur(1,0);
		lcd_send_string("LOI TAO KI TU");
		return -1;
	}	
	//HAL_Delay(1000);
	//HAL_Delay(10);
	
	p = fingerFastSearch();
  if (p != FINGERPRINT_OK)
	{
		/*lcd_clear();
		lcd_put_cur(0,0);
		lcd_send_string("  VAN TAY SAI  ");
		lcd_put_cur(1,0);
		lcd_send_string("XIN HAY THU LAI");
		HAL_Delay(1000);*/
		return -1;
	}
  return fingerID; 
}
