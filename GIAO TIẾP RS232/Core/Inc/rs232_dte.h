/*
 * rs232_dte.h
 *
 *  Created on: Apr 2, 2025
 *      Author: PC
 */

#ifndef SRC_RS232_DTE_H_
#define SRC_RS232_DTE_H_



#endif /* SRC_RS232_DTE_H_ */
#include "stm32f1xx_hal.h"

#define DCD_Pin GPIO_PIN_1
#define DTR_Pin GPIO_PIN_2
#define DSR_Pin GPIO_PIN_3
#define RTS_Pin GPIO_PIN_4
#define CTS_Pin GPIO_PIN_5
#define DCD_GPIO_Port GPIOA
#define DTR_GPIO_Port GPIOA
#define DSR_GPIO_Port GPIOA
#define RTS_GPIO_Port GPIOA
#define CTS_GPIO_Port GPIOA

void intRS232_DTE();
void Receive_dte(uint8_t *data,uint8_t *rec,uint8_t st,uint32_t time);
