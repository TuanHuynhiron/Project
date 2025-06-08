/*
 * rs232_dce.c
 *
 *  Created on: Apr 2, 2025
 *      Author: PC
 */

#include <rs232_dce.h>
#include "stm32f1xx_hal.h"
#include "main.h"
extern UART_HandleTypeDef huart1;
void intRS232_DCE(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(DCD_GPIO_Port, DCD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CTS_GPIO_Port, CTS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RI_GPIO_Port, RI_Pin, GPIO_PIN_RESET);
	//DCD pin
	GPIO_InitStruct.Pin = DCD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DCD_GPIO_Port, &GPIO_InitStruct);
	//DTR Pin
	GPIO_InitStruct.Pin = DTR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DTR_GPIO_Port, &GPIO_InitStruct);
	//CTS Pin
	GPIO_InitStruct.Pin = CTS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(CTS_GPIO_Port, &GPIO_InitStruct);
	//RI Pin
	GPIO_InitStruct.Pin = RI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(RI_GPIO_Port, &GPIO_InitStruct);
	//INPUT
	//DSR Pin
	GPIO_InitStruct.Pin = DSR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DSR_GPIO_Port, &GPIO_InitStruct);
	//RTS Pin
	GPIO_InitStruct.Pin = RTS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(RTS_GPIO_Port, &GPIO_InitStruct);
}
void Transmit_dce(uint8_t *data,uint8_t *rec,uint16_t time){
	uint8_t i=0;
	time=HAL_GetTick();
	while ( ((HAL_GetTick()-time)<500) && (i==0) )	i=HAL_GPIO_ReadPin(DTR_GPIO_Port, DTR_Pin);
	if (i==1) {
		HAL_GPIO_WritePin(DSR_GPIO_Port, DSR_Pin, 1);
		HAL_GPIO_WritePin(DCD_GPIO_Port, DCD_Pin, 1);
		HAL_Delay(10);
		HAL_UART_Transmit_IT(&huart1, data, 8);
		HAL_GPIO_WritePin(DCD_GPIO_Port, DCD_Pin, 0);
		time=HAL_GetTick();i=0;
		while ( ((HAL_GetTick()-time)<500) && (i==0) )	i=HAL_GPIO_ReadPin(RTS_GPIO_Port, RTS_Pin);
		if (i==1){
			HAL_GPIO_WritePin(CTS_GPIO_Port, CTS_Pin, 1);
			HAL_Delay(10);
			HAL_UART_Receive_IT(&huart1, rec, 8);
			time=HAL_GetTick();
			while ( ((HAL_GetTick()-time)<500) && (i!=0) )	i=HAL_GPIO_ReadPin(RTS_GPIO_Port, RTS_Pin);
			if(i==0){
				HAL_GPIO_WritePin(CTS_GPIO_Port, CTS_Pin, 0);
				HAL_GPIO_WritePin(DCD_GPIO_Port, DCD_Pin, 0);
				time=HAL_GetTick();i=1;
				while ( ((HAL_GetTick()-time)<500) && (i==0) )	i=HAL_GPIO_ReadPin(DTR_GPIO_Port, DTR_Pin);
				if (i==0){
					HAL_GPIO_WritePin(DSR_GPIO_Port, DSR_Pin, 0);
				}
			}
		}
	}
}
