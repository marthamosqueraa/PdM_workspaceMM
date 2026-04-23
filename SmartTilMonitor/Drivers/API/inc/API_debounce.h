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
#include <stdbool.h>

typedef bool bool_t;

//Tipos de Push en button1
typedef enum{
	NO_PRESSED,
	SHORT_PRESSED,
	LONG3_PRESSED
}pressType_t;

//Funciones
void debounceFSM_init(void);
void debounceFSM_update(void);

pressType_t readKey(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
