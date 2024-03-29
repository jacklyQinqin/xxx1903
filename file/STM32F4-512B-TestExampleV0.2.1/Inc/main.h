/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED0_Pin GPIO_PIN_3
#define LED0_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOE
#define CHIPRST_Pin GPIO_PIN_1
#define CHIPRST_GPIO_Port GPIOA
#define HANKSHAKE_Pin GPIO_PIN_3
#define HANKSHAKE_GPIO_Port GPIOA
#define SPI_SS_Pin GPIO_PIN_4
#define SPI_SS_GPIO_Port GPIOA
#define NAC_Pin GPIO_PIN_4
#define NAC_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_13
#define KEY1_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_14
#define KEY2_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_15
#define KEY3_GPIO_Port GPIOD
#define KEY4_Pin GPIO_PIN_6
#define KEY4_GPIO_Port GPIOC
#define BEEP_Pin GPIO_PIN_0
#define BEEP_GPIO_Port GPIOE
#define GP28_Pin GPIO_PIN_1
#define GP28_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
