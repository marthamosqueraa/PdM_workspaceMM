Practica 5 - Punto 1

Se implementa modulo de Software en archivos fuente API_uart.c y API_uart.h

En API_uart.h se deben ubicar los prototipos de las funciones públicas.
bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

En API_uart.c se deben ubicar los prototipos de las funciones privadas y la implementación de todas las funciones de módulo, privadas y públicas.


Comentarios para la Verificacion:
1. Instalar un monitor serie externo (Ej: Putty)

Una vez instalado, se debe configurar:
Connection type: Serial (radio button)
Serial line: /dev/ttyACM0 (identificado al correr ls /dev/ttyACM* en el terminal)
Speed: 9600

2. Alternativa en Linux (Screen)

sudo apt install screen
sudo screen /dev/ttyACM0 9600

Para salir de screen es necesario: Ctrl + A, luego presionar la letra k y finalmente y



