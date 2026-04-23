#include "main.h"

//Prototipos de funciones
//Inicializan, envío de comandos (cmd), envío de datos, envío de cadena de
//caracteres y visualización de nros float en el LCD
void APILCD_init (void);
void APILCD_sendcmd (char cmd);
void APILCD_sendData (char data);
void APILCD_sendstring (char *str);
void APILCD_sendfloat (float data, int numberofdigits);
void APILCD_clear(void);


