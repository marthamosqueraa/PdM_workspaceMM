/*
 * API_delay.h
 *
 *  Created on: Mar 19, 2026
 *      Author: lilo
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h> //Para uint32_t
#include <stdbool.h> //Para bool
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

// Tipos para el manejo de retardos no bloqueantes DEFINICIÓN
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;


/* Exported functions prototypes. Esto es la firma de las funciones---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);
bool_t delayIsRunning(delay_t * delay);

/* USER CODE END EFP */



#endif /* API_INC_API_DELAY_H_ */
