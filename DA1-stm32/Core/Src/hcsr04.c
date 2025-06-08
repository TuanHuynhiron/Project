/*
 * hcsr04.c
 *
 *  Created on: Jan 28, 2024
 *      Author: Administrator
 */

#include "hcsr04.h"
#include "main.h"
#include "stm32f1xx_hal.h"

uint32_t delay(TIM_HandleTypeDef *htim , uint32_t us)
{
	uint32_t time_delay = __HAL_TIM_GET_COUNTER(htim);
	while ((__HAL_TIM_GET_COUNTER(htim) - time_delay) <= us);
	__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
	return 1;
}
void HCSR04_Read (TIM_HandleTypeDef *htim, uint32_t *check)
{

	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	__HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
	*check = delay(htim,10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

}


// Bo ham nay truoc main
/*
uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;  // is the first value captured ?
double Distance  = 0.0;
 void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
	{
		if (Is_First_Captured==0) // if the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // if the first is already captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			if (IC_Val2 > IC_Val1)
						{
							Difference = IC_Val2-IC_Val1;
						}

						else if (IC_Val1 > IC_Val2)
						{
							Difference = (0xffff - IC_Val1) + IC_Val2;
						}

						Distance = (double)Difference * .034/2;
						Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}
 */
// Bo dong nay trc while
//HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1); // khoi dong timer
