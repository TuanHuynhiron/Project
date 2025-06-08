/*
 * R-W-SD.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Administrator
 */

#ifndef INC_CLOCK_TT_H_R_W_SD_H_
#define INC_CLOCK_TT_H_R_W_SD_H_
#include "main.h"
#include "fatfs.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
#define start 0x0800F000
void write_sd(const TCHAR* fodler,const TCHAR* txt,char *buffer, uint8_t *bt,uint8_t size);
void phatnhac();
void readdanhsachnhac(const TCHAR* fodler,const TCHAR* txt,char *buffer);
void readline(const TCHAR* fodler,const TCHAR* txt,char *buffer,uint8_t *mang, uint16_t size,uint32_t *a,uint8_t *mode);
void readsdtxt(const TCHAR* fodler,const TCHAR* txt,char *buffer,uint8_t size);
void str2uint8t(char *str, uint8_t *mang, uint8_t size);
//void phatnhac(uint8_t *amthanh,uint8_t lan,uint16_t size);

#endif /* INC_CLOCK_TT_H_R_W_SD_H_ */
