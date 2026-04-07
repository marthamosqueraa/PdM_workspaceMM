#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stddef.h>

#define SIZE_MIN 1
#define SIZE_MAXIMO 256
#define UART_TIMEOUT 1000


//Variable static privada para este modulo
static UART_HandleTypeDef huart2;

bool_t uartInit() //Realiza inicializacion de UART.
	{ //imprime msj con sus parametros de condiguracion.
	  huart2.Instance = USART2;

	  huart2.Init.BaudRate = 9600;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {return false;}

	  //Funcion para imprimir mensaje cuando inicializacion = ok
	  uint8_t confirmationMessage[] = "Inicializacion OK - BaudRate 9600\r\n";
	  uartSendString(confirmationMessage);

	  return true;
	}


void uartSendString(uint8_t * pstring){
	//recibe un puntero a un string que se desea enviar por la UART
	//completo (hasta el caracter ‘\0’) y debe utilizar la función
	//de la HAL HAL_UART_Transmit(...) para transmitir el string.
	if(pstring==NULL){return;} //valido que el puntero  no sea NULL

	//calculo el tamanio hasta el '\0'
	uint16_t size = strlen((char*)pstring);

	if(size>= SIZE_MIN && size<= SIZE_MAXIMO){

		//Verifico HAL_UART_Transmit(...) y la utilizo
		if (HAL_UART_Transmit(&huart2, pstring, (uint16_t)size, UART_TIMEOUT) != HAL_OK)
		{

		}
	}


}

void uartSendStringSize(uint8_t * pstring, uint16_t size){
	if(pstring==NULL){return;} //valido que el puntero  no sea NULL

	if(size>= SIZE_MIN && size<= SIZE_MAXIMO){//size debe estar entre 1-255

		if (HAL_UART_Transmit(&huart2, pstring, (uint16_t)size, UART_TIMEOUT) != HAL_OK)
		{

		}
	}

//	HAL_UART_Transmit((&huart2,pstring,size,1000)=="HAL_OK");//1000 ES TIMEOUT
}


void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	if(pstring==NULL){return;}

	if(size>= SIZE_MIN && size<= SIZE_MAXIMO){//size debe estar entre 1-255

			if (HAL_UART_Receive(&huart2, pstring, size, UART_TIMEOUT) != HAL_OK)
			{

			}
		}

}
