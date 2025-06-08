/*
 * hcsr04.h
 *
 *  Created on: Jan 28, 2024
 *      Author: Administrator
 */

#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_
#include "main.h"
#include "stm32f1xx_hal.h"

#define TRIG_PIN trig_Pin
#define TRIG_PORT trig_GPIO_Port
uint32_t delay(TIM_HandleTypeDef *htim , uint32_t us);
void HCSR04_Read (TIM_HandleTypeDef *htim, uint32_t *check);


#endif /* INC_HCSR04_H_ */
