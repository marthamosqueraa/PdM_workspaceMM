Practica 4 - Punto 1:  
En base a Practica 3, se implementa una MEF antirebote que lee el estado del pulsador de la placa STM32F446RE

El estado inicial de la MEF es BUTTON_UP.

En main.c se implementan las siguientes funciones:
void debounceFSM_init();		//Para cargar el estado inicial
void debounceFSM_update();	// Para leer entradas, manejar lógica de transición de estados y actualización las salidas
void buttonPressed();			// Enciende el LED
void buttonReleased();		// Apaga el LED 

El tiempo de anti-rebote debe ser de 40 ms con un retardo no bloqueante.

La función *debounceFSM_update()* se debe llamar periódicamente.

Se define una enumeración debounceState_t  con los posibles estados de la MEF:

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t
