/*
 * rs232_dce.h
 *
 *  Created on: Apr 2, 2025
 *      Author: PC
 */

#ifndef SRC_RS232_DCE_H_
#define SRC_RS232_DCE_H_



#endif /* SRC_RS232_DCE_H_ */
#include "stm32f1xx_hal.h"

#define DCD_Pin GPIO_PIN_1
#define DTR_Pin GPIO_PIN_2
#define DSR_Pin GPIO_PIN_3
#define RTS_Pin GPIO_PIN_4
#define CTS_Pin GPIO_PIN_5
#define RI_Pin GPIO_PIN_6
#define DCD_GPIO_Port GPIOA
#define DTR_GPIO_Port GPIOA
#define DSR_GPIO_Port GPIOA
#define RTS_GPIO_Port GPIOA
#define CTS_GPIO_Port GPIOA
#define RI_GPIO_Port GPIOA


void intRS232_DCE();
void Transmit_dce(uint8_t *data,uint8_t *rec,uint16_t time);
