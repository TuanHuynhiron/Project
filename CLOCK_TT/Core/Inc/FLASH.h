/*
 * FLASH.h
 *
 *  Created on: Nov 15, 2023
 *      Author: ACER
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32f1xx.h"

void flash_unlock ();

void flash_lock ();

void flash_eraser(uint32_t addr);

void flash_write_arr(uint32_t addr, uint8_t *data, uint16_t length);

void flash_read(uint32_t addr, uint8_t *data, uint16_t length);

#endif /* INC_FLASH_H_ */
