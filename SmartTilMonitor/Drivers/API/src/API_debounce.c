/*
 * API_debounce.c
 *
 *  Created on: Mar 29, 2026
 *      Author: Martha Mosquera
 */
//#include "stm32f4xx_hal.h"
#include "main.h"
#include "API_debounce.h"
#include <stdio.h>

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static debounceState_t currentState;
static pressType_t eventLastPress;

static delay_t debounceDelay;
static delay_t longPressDelay;
static bool_t longPressDetected;


void debounceFSM_init()
{
	currentState = BUTTON_UP;
	eventLastPress = NO_PRESSED;
	longPressDetected = false;

	delayInit(&debounceDelay, 40);
	delayInit(&longPressDelay, 3000);
}

////////////////
//void buttonPressed(void){
//	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//}//Enciende el LED
//
//void buttonReleased(void){
//	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//}	//Apaga el LED
///////////////

void debounceFSM_update()
{
	bool_t button_down = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);

	switch(currentState)
	{

	case BUTTON_UP:
		if(button_down)
		{
			currentState = BUTTON_FALLING;
			delayInit(&debounceDelay, 40);
		}
		break;

	case BUTTON_FALLING:
		if(delayRead(&debounceDelay))
		{
			if(button_down)
			{
				currentState = BUTTON_DOWN;
				longPressDetected = false;
				delayInit(&longPressDelay, 3000);

			} else{
				currentState = BUTTON_UP;
			}
		}
	break;

	case BUTTON_DOWN:
		if(!button_down)
		{
			//Se suelta el button
			if(longPressDetected)
			{
				eventLastPress = LONG3_PRESSED;
			}else
				{
				eventLastPress = SHORT_PRESSED;
				}

				currentState = BUTTON_RAISING;
				delayInit(&debounceDelay, 40);
		}
		else if(delayRead(&longPressDelay))
			{
			longPressDetected = true;
			}
	break;

	case BUTTON_RAISING:
		if(delayRead(&debounceDelay))
		{
			currentState = BUTTON_UP;
		}

	break;

	default:
		debounceFSM_init(); //Initial State de MEF es BUTTON_UP
	break;

	}
}	//Para leer las entradas, resolver lógica de transición de estados y actualizar las salidas

pressType_t readKey(void)
{
	pressType_t status = eventLastPress;
	eventLastPress = NO_PRESSED;
	return status;
}


