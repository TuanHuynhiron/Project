/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define OK_Pin GPIO_PIN_1
#define OK_GPIO_Port GPIOA
#define OK_EXTI_IRQn EXTI1_IRQn
#define TRAI_Pin GPIO_PIN_2
#define TRAI_GPIO_Port GPIOA
#define TRAI_EXTI_IRQn EXTI2_IRQn
#define PHAI_Pin GPIO_PIN_3
#define PHAI_GPIO_Port GPIOA
#define PHAI_EXTI_IRQn EXTI3_IRQn
#define cs_Pin GPIO_PIN_4
#define cs_GPIO_Port GPIOA
#define dc_Pin GPIO_PIN_0
#define dc_GPIO_Port GPIOB
#define rs_Pin GPIO_PIN_1
#define rs_GPIO_Port GPIOB
#define SD_CARD_Pin GPIO_PIN_12
#define SD_CARD_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_10
#define DHT11_GPIO_Port GPIOA
#define DUOI_Pin GPIO_PIN_4
#define DUOI_GPIO_Port GPIOB
#define DUOI_EXTI_IRQn EXTI4_IRQn
#define TREN_Pin GPIO_PIN_5
#define TREN_GPIO_Port GPIOB
#define TREN_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
