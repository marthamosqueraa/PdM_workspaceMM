/*
 * API_debounce.h
 *
 *  Created on: Mar 29, 2026
 *      Author: Martha Mosquera
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef bool bool_t;

void debounceFSM_init();
void debounceFSM_update();

bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
