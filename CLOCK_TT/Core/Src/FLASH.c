/*
 * FLASH.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ACER
 */
#include "FLASH.h"

void flash_unlock ()
{
	HAL_FLASH_Unlock();
}

void flash_lock ()
{
	HAL_FLASH_Lock();
}

void flash_eraser(uint32_t addr)
{
	FLASH_EraseInitTypeDef eraserInit;
	eraserInit.NbPages = 1;
	eraserInit.PageAddress = addr;
	eraserInit.TypeErase = FLASH_TYPEERASE_PAGES;
	uint32_t Page_Error; // bien nay khong co y nghia gi, them vao cho du tham so
	HAL_FLASHEx_Erase(&eraserInit, &Page_Error);

}

void flash_write_arr(uint32_t addr, uint8_t *data, uint16_t length)
{
	for(uint16_t i=0; i<=length; i+=2)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, data[i]|((uint16_t)data[i+1]<<8));
	}
}

void flash_read(uint32_t addr, uint8_t *data, uint16_t length)
{
	for(uint16_t i=0; i<=length; i+=2)
	{
		uint16_t data_temp = *(volatile uint32_t *)(addr + i);
		*(data + i) = data_temp;
		*(data + i + 1) = data_temp>>8;
	}
}

