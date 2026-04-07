/*
 * API_delay.c
 *
 *  Created on: Mar 19, 2026
 *      Author: lilo
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN PV */
//delay_t ledDelay;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void delayInit( delay_t * delay, tick_t duration_){ //Configuración exclusivamente
	if(delay == NULL ) return;
	delay -> duration = duration_;
	delay -> running = false;
}

bool_t delayRead(delay_t * delay){
	if(delay == NULL ) return false;

	if(delay->running==false){
		delay-> startTime = HAL_GetTick();
		delay -> running = true;
		return false;

	} else{
		if((HAL_GetTick() - delay-> startTime) >= delay -> duration){
			delay->running=false;
			return true;
		}
	}
	return false;
}

bool_t delayIsRunning(delay_t * delay){
	if(delay == NULL ) return false;
			return delay->running;
}

void delayWrite(delay_t * delay, tick_t duration_){
	if(delay == NULL ) return;

	delay -> duration = duration_;
}

