/*
 * DHT11.h
 *
 *  Created on: Dec 31, 2023
 *      Author: Administrator
 */

#ifndef INC_CLOCK_TT_H_DHT11_H_
#define INC_CLOCK_TT_H_DHT11_H_
#include "stm32f1xx_hal.h"
#define DHT11_STARTTIME 18000
#define DHT22_STARTTIME 12000
#define DHT11 0x01
#define DHT22 0x02
typedef struct
{
	uint16_t Type;
	TIM_HandleTypeDef* Timer;
	uint16_t Pin;
	GPIO_TypeDef* PORT;
	uint8_t Temp;
	uint8_t  Humi;
}DHT_Name;
void DELAY_TIM_Init(TIM_HandleTypeDef *htim);
void DELAY_TIM_Us(TIM_HandleTypeDef *htim, uint16_t time);
void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time);
void DHT_Init(DHT_Name* DHT, uint8_t DHT_Type, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
uint8_t DHT_ReadTempHum(DHT_Name* DHT);
//uint8_t docDHT(DHT_Name DHT, uint8_t DHT_Type, TIM_HandleTypeDef Timer, GPIO_TypeDef DH_PORT, uint16_t DH_Pin);

#endif /* INC_CLOCK_TT_H_DHT11_H_ */
