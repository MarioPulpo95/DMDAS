/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define HCSR_TRIG_Pin GPIO_PIN_0
#define HCSR_TRIG_GPIO_Port GPIOA
#define USART2_TX_Pin GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define USART2_RX_Pin GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define MOTOR_DIRECTION_Pin GPIO_PIN_4
#define MOTOR_DIRECTION_GPIO_Port GPIOA
#define HCSR_ECHO_Pin GPIO_PIN_6
#define HCSR_ECHO_GPIO_Port GPIOA
#define PLC_EN_MOT_Pin GPIO_PIN_0
#define PLC_EN_MOT_GPIO_Port GPIOB
#define PLC_DIRECTION_Pin GPIO_PIN_8
#define PLC_DIRECTION_GPIO_Port GPIOA
#define MOTOR_ENABLE_Pin GPIO_PIN_10
#define MOTOR_ENABLE_GPIO_Port GPIOA
#define PLC_PulsePort_Pin GPIO_PIN_11
#define PLC_PulsePort_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define END_STOPmin_Pin GPIO_PIN_4
#define END_STOPmin_GPIO_Port GPIOB
#define END_STOPmin_EXTI_IRQn EXTI4_IRQn
#define END_STOPmax_Pin GPIO_PIN_5
#define END_STOPmax_GPIO_Port GPIOB
#define END_STOPmax_EXTI_IRQn EXTI9_5_IRQn
#define MOTOR_STEP_Pin GPIO_PIN_6
#define MOTOR_STEP_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_8
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
