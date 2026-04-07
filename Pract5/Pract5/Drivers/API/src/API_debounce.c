/*
 * API_debounce.c
 *
 *  Created on: Mar 29, 2026
 *      Author: Martha Mosquera
 */
#include "API_debounce.h"
//#include "stm32f4xx_hal.h"
#include "API_delay.h"
#include "main.h"

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static debounceState_t currentState;
static bool_t button_down, keyPressed;
static delay_t debounceDelay;

void debounceFSM_init(){
	currentState = BUTTON_UP;
	keyPressed = false;
	delayInit(&debounceDelay, 40);
}		//Carga el estado inicial: BUTTON_UP. Inicializo el temp de la MEF

void buttonPressed(void){
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}//Enciende el LED

void buttonReleased(void){
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}	//Apaga el LED

void debounceFSM_update(){
	button_down = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);
	switch(currentState)
	{

	case BUTTON_UP:
		if(button_down){
			currentState = BUTTON_FALLING;
		}
		break;

	case BUTTON_FALLING:
		if(delayRead(&debounceDelay)){
			if(button_down){
				currentState = BUTTON_DOWN;
				keyPressed = true;//deteccion de flanco desc.
				buttonPressed(); //Se emite el evento: Encender LED
			} else{
				currentState = BUTTON_UP;
			}
		}
		break;

	case BUTTON_DOWN:
		if(!button_down){//Esto equivale a detectar un Flanco Ascend.
			currentState = BUTTON_RAISING;
		}
		break;

	case BUTTON_RAISING:
		if(delayRead(&debounceDelay)){
			if(!button_down){
				currentState = BUTTON_UP;
				buttonReleased(); //Emite el evento: Apagar LED
			}else{
				currentState = BUTTON_DOWN;
			}
		}

		break;

	default:
		debounceFSM_init(); //Initial State de MEF es BUTTON_UP
		break;

	}
}	// Lee las entradas, resuelve la lógica de transición de estados y actualiza las salidas

bool_t readKey(){
	bool_t status = keyPressed;
	keyPressed = false; //reset posterior a la lectura.
	return status;
}


