/*
 * R-W-SD.c
 *
 *  Created on: Dec 30, 2023
 *      Author: Administrator
 */
#include "fatfs.h"
#include "stm32f1xx_hal.h"
#include "stm32_hal_legacy.h"
#include "main.h"
#include <clock_TT-h/R-W-SD.h>
#include <clock_TT-h/ssd1306.h>
#include <clock_TT-h/ssd1306_tests.h>
#include <clock_TT-h/DHT11.h>
FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result

UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

void phatnhac()
{
	HAL_TIM_Base_Start_IT(&htim3);
}
void readsdtxt(const TCHAR* fodler,const TCHAR* txt,char *buffer,uint8_t size)
{
	fresult = f_mount(&fs, fodler, 0);
	if (fresult != FR_OK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	fresult = f_open(&fil, txt, FA_READ);
	f_gets(buffer,size,&fil);
	fresult = f_close(&fil);
}
void str2uint8t(char *str, uint8_t *mang, uint8_t size)
{
	for (int i=0;i<size;i++){
		*(mang+i)=(*(str +i*2)-48)*16+(*(str +i*2+1)-48);
	}
}
void readline(const TCHAR* fodler,const TCHAR* txt,char *buffer,uint8_t *mang, uint16_t size,uint32_t *a,uint8_t *mode)
{

	fresult = f_mount(&fs, fodler, 0);
	if (fresult != FR_OK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	fresult = f_open(&fil, txt, FA_READ);
	while ((f_gets(buffer,size*2+1 , &fil) != NULL ))
	{
		for (int i=0;i<size;i++){
			*(mang + i)=(*(buffer + i*2)-48)*16+(*(buffer+i*2+1)-48);
		}
		phatnhac();
		while (*a!=1);
		if (*mode!=0) {
			*mode=0;
			break;
		}
	}
//	fresult = f_close(&fil);
}
void readdanhsachnhac(const TCHAR* fodler,const TCHAR* txt,char *buffer)
{
	uint8_t a;
	a=0;
	fresult = f_mount(&fs, fodler, 0);
	if (fresult != FR_OK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	fresult = f_open(&fil, txt, FA_READ);
	while ((f_gets(buffer,50 , &fil) != NULL )||((a+18+10)>64))
	{
		ssd1306_SetCursor(0,18+a);
		ssd1306_WriteString(buffer,Font_7x10,White);
		ssd1306_UpdateScreen();
		a+=10;
	}
	fresult = f_close(&fil);
}

void write_sd(const TCHAR* fodler,const TCHAR* txt,char *buffer, uint8_t *bt,uint8_t size)
{
	fresult = f_mount(&fs, fodler, 0);
	if (fresult != FR_OK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	fresult = f_open(&fil, txt, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	for(int i=0;i<size;i++){
		*(buffer+2*i)=(*(bt+i)/16) +48;
		*(buffer+2*i+1)=(*(bt+i)%16 )+48;
	}
	f_puts(buffer, &fil);
	fresult = f_close(&fil);
}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //16Khz timer
//  	{
//
//  	   TIM2->CCR1=amthanh[cnt++];
//  	   if(cnt>=sizeof(amthanh)) {
//  		   cnt=1;
//  		 HAL_TIM_Base_Stop_IT(&htim3);
//  		 a=1;
//  	   }
//  	}







//voide vd(){
//	fresult = f_mount(&fs, "", 0);
//		if (fresult != FR_OK) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//		/* Check free space */
//		f_getfree("", &fre_clust, &pfs);
//
//		total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
//
//		free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
//
//		/* Open file to write/ create a file if it doesn't exist */
//	    fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
//
//		/* Writing text */
//
//
//		/* Close file */
//		fresult = f_close(&fil);
//		fresult = f_open(&fil, "ff1.txt", FA_READ );
//		f_gets(buffer,fil.fsize,&fil);
//		fresult = f_close(&fil);
//}
