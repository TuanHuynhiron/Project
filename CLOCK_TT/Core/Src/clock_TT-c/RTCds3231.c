/*
 * RTCds3231.c
 *
 *  Created on: Dec 30, 2023
 *      Author: Administrator
 */
#include <clock_TT-h/RTCds3231.h>
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal.h"




void readtime(uint8_t *u8_revBuffer,uint8_t id)
{
		HAL_I2C_Mem_Read_IT(&hi2c1,id,0x00, 0x01U, u8_revBuffer,7 );
}

uint8_t BCD2DEC(uint8_t date)
{
	return (date>>4)*10+ (date&0x0f);
}

uint8_t DEC2BCD(uint8_t date)
{
	return (date/10)<<4|(date%10);
}

uint8_t stactusTX(){
	uint8_t id;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  for (uint8_t i=0; i<255; i++)
  {
	  if (HAL_I2C_IsDeviceReady(&hi2c1, i,1 ,10)==HAL_OK)
	  {
		  id =i;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	  }
  }
  return id;
}
void settime(uint8_t *TX, uint8_t id)
{

			*(TX +0)=DEC2BCD(*(TX +0));//s
			*(TX +1)=DEC2BCD(*(TX +1));//min
			*(TX +2)=DEC2BCD(*(TX +2));//hour
			*(TX +3)=DEC2BCD(*(TX +3));//thu
			*(TX +4)=DEC2BCD(*(TX +4));//ngay
			*(TX +5)=DEC2BCD(*(TX +5));//thang
			*(TX +6)=DEC2BCD(*(TX +6));//nam
	  	    HAL_I2C_Mem_Write_IT(&hi2c1,id,0x00, 0x00000001U, TX,7);
}
