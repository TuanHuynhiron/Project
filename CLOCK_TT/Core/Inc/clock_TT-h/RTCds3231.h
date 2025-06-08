/*
 * RTCds3231.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Administrator
 */

#ifndef INC_CLOCK_TT_H_RTCDS3231_H_
#define INC_CLOCK_TT_H_RTCDS3231_H_
#include "main.h"
#include "stdio.h"
extern I2C_HandleTypeDef hi2c1;

typedef struct
{
	uint8_t hours;
	uint8_t min;
	uint8_t sec;
	uint8_t date;
	uint8_t day;
	uint8_t month;
	uint8_t year;

}DS3231_typedef;

void readtime(uint8_t *u8_revBuffer,uint8_t id);
uint8_t BCD2DEC(uint8_t date);
uint8_t DEC2BCD(uint8_t date);
uint8_t stactusTX();
void settime(uint8_t *TX, uint8_t id);
#endif /* INC_CLOCK_TT_H_RTCDS3231_H_ */
