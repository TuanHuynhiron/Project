/*
 * manhinh.h
 *
 *  Created on: Jan 1, 2024
 *      Author: Administrator
 */

#ifndef INC_CLOCK_TT_H_MANHINH_H_
#define INC_CLOCK_TT_H_MANHINH_H_
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <clock_TT-h/RTCds3231.h>
#include <clock_TT-h/DHT11.h>
#include <clock_TT-h/R-W-SD.h>

void manhinhchinh(uint8_t id,DS3231_typedef *DOC,uint8_t *buffer,DHT_Name* DHT,char giophut[5],char giay[4],char doC[4],char doAm[3] );
void manhinhphatnhac();
#endif /* INC_CLOCK_TT_H_MANHINH_H_ */
