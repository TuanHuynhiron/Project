/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define a_Pin GPIO_PIN_1
#define a_GPIO_Port GPIOA
#define p1_Pin GPIO_PIN_2
#define p1_GPIO_Port GPIOA
#define p2_Pin GPIO_PIN_3
#define p2_GPIO_Port GPIOA
#define p4_Pin GPIO_PIN_4
#define p4_GPIO_Port GPIOA
#define p3_Pin GPIO_PIN_5
#define p3_GPIO_Port GPIOA
#define res_Pin GPIO_PIN_8
#define res_GPIO_Port GPIOA
#define dc_Pin GPIO_PIN_9
#define dc_GPIO_Port GPIOA
#define cs_Pin GPIO_PIN_10
#define cs_GPIO_Port GPIOA
#define echo_Pin GPIO_PIN_11
#define echo_GPIO_Port GPIOA
#define trig_Pin GPIO_PIN_12
#define trig_GPIO_Port GPIOA
#define R_Pin GPIO_PIN_8
#define R_GPIO_Port GPIOB
#define L_Pin GPIO_PIN_9
#define L_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
